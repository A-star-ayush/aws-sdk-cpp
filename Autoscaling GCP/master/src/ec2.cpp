#include <aws/core/Aws.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/auth/AWSCredentialsProvider.h>

#include <aws/ec2/EC2Client.h>
#include <aws/ec2/model/DescribeInstancesRequest.h>
#include <aws/ec2/model/DescribeInstancesResponse.h>

#include <aws/ec2/model/StartInstancesRequest.h>
#include <aws/ec2/model/StartInstancesResponse.h>

#include <aws/ec2/model/CreateTagsRequest.h>
#include <aws/ec2/model/RunInstancesRequest.h>
#include <aws/ec2/model/RunInstancesResponse.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <iostream>

#include "../headers/ec2.h"
#include "../headers/utility.h"

myAccount* account(const char* access_key_id, const char* secret_access_key, const char* region) {
	myAccount* acc = (myAccount*)malloc(sizeof(myAccount));

	strcpy(acc->key1, access_key_id);
	strcpy(acc->key2, secret_access_key);
	strcpy(acc->region, region);

	return acc; 
}

std::vector<Instance*> getInstances(myAccount* acc) {
	
	std::vector<Instance*> ins;

	Aws::SDKOptions options;
	options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;
	options.loggingOptions.defaultLogPrefix = "aws_getInstances";

	Aws::InitAPI(options);
	{
		Aws::Client::ClientConfiguration config;
		config.region = acc->region;

		Aws::EC2::EC2Client ec2(Aws::Auth::AWSCredentials(acc->key1, acc->key2), config);
		Aws::EC2::Model::DescribeInstancesRequest request;

		while(1) {
			auto outcome = ec2.DescribeInstances(request);
			if (outcome.IsSuccess()) {
				const auto &reservations =
					outcome.GetResult().GetReservations();

				for (const auto &reservation : reservations) {
					const auto &instances = reservation.GetInstances();
					for (const auto &instance : instances) {
						int state = instance.GetState().GetCode();
						const char* id = instance.GetInstanceId().c_str();
						const char* ip = instance.GetPublicIpAddress().c_str();
						
						Instance* ptr = (Instance*)calloc(1, sizeof(Instance));
						ptr->state = state;
						strcpy(ptr->id, id);
						strcpy(ptr->ip, ip);
						ins.push_back(ptr);

						if(state == 80)  // 16 for running
							acc->stopped = acc->stopped + 1;
						else if(state != 16)
							acc->others = acc->others + 1;
						acc->total = acc->total + 1;
					}
				}

				if (outcome.GetResult().GetNextToken().size() > 0) {
					request.SetNextToken(outcome.GetResult().GetNextToken());
				} else
					break;
			} else
				exit_err("Failed to describe ec2 instances");	
		}
	}
	Aws::ShutdownAPI(options);

	return ins;
}


void startInstances(myAccount* acc, std::vector<Instance*>& ins, int n) {

	if(n == 0)
		return;

	Aws::SDKOptions options;
	options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;
	options.loggingOptions.defaultLogPrefix = "aws_startInstances";

	Aws::InitAPI(options);
	{
		Aws::Client::ClientConfiguration config;
		config.region = acc->region;

		Aws::EC2::EC2Client ec2(Aws::Auth::AWSCredentials(acc->key1, acc->key2), config);
		Aws::EC2::Model::StartInstancesRequest start_request;
		for(Instance* i : ins) {
			if(n == 0)
				break;
			if(i->state == 80) {
				--n;
				start_request.AddInstanceIds(i->id);
				acc->stopped = acc->stopped - 1;
			}
		}

		auto start_instancesOutcome = ec2.StartInstances(start_request);

		if (!start_instancesOutcome.IsSuccess())
    		exit_err("Failed to start instances");
		else 
    		printf("Successfully started instances\n");
	}
	Aws::ShutdownAPI(options);
}

void createInstances(myAccount* acc, std::vector<Instance*>& ins, int n) {

	if(n == 0)
		return;

	Aws::SDKOptions options;
	options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;
	options.loggingOptions.defaultLogPrefix = "aws_createInstances";

	Aws::InitAPI(options);
	{
		Aws::Client::ClientConfiguration config;
		config.region = acc->region;

		Aws::EC2::EC2Client ec2(Aws::Auth::AWSCredentials(acc->key1, acc->key2), config);
		Aws::String ami_id = "ami-d8b2ffb7";
		
		Aws::EC2::Model::RunInstancesRequest run_request;
		run_request.SetImageId(ami_id);
		run_request.SetInstanceType(Aws::EC2::Model::InstanceType::t2_micro);
		run_request.SetMinCount(n);
		run_request.SetMaxCount(n);
		run_request.SetKeyName("key_pair_1");
		run_request.AddSecurityGroupIds("sg-01440069");

		auto run_outcome = ec2.RunInstances(run_request);
		if (!run_outcome.IsSuccess())
    		exit_err("Failed to start ec2 instance");
		
		const auto& instances = run_outcome.GetResult().GetInstances();
		for (const auto &instance : instances) {
			const char* id = instance.GetInstanceId().c_str();
			const char* ip = instance.GetPublicIpAddress().c_str();
						
			Instance* ptr = (Instance*)calloc(1, sizeof(Instance));
			ptr->state = 16;
			strcpy(ptr->id, id);
			strcpy(ptr->ip, ip);
			ins.push_back(ptr);
			
			acc->total = acc->total + 1;
		}
	}
	Aws::ShutdownAPI(options);
}