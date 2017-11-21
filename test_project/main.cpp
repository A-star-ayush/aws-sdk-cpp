#include <aws/core/Aws.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/ec2/EC2Client.h>

#include <aws/ec2/model/CreateTagsRequest.h>
#include <aws/ec2/model/RunInstancesRequest.h>
#include <aws/ec2/model/RunInstancesResponse.h>

#include <aws/ec2/model/StopInstancesRequest.h>
#include <aws/ec2/model/StopInstancesResponse.h>

#include <aws/ec2/model/StartInstancesRequest.h>
#include <aws/ec2/model/StartInstancesResponse.h>

#include <iostream>

int main(int argc, char** argv)
{
	
	/*// Starting instances
	
	Aws::SDKOptions options;
	options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;

	Aws::InitAPI(options);
	{
		Aws::String instance_id = "i-0742416418d041e07";
		
		Aws::Client::ClientConfiguration config;
		config.region = Aws::Region::AP_SOUTH_1;

		Aws::EC2::EC2Client ec2(Aws::Auth::AWSCredentials("AKIAITDKO3FURSYIACUQ", "Aq38PvhZVDwbinAx9GXnh07153SoL9gv9z4STHuM"), config);

		Aws::EC2::Model::StartInstancesRequest start_request;
		start_request.AddInstanceIds(instance_id);
		start_request.SetDryRun(true);

		auto dry_run_outcome = ec2.StartInstances(start_request);
		assert(!dry_run_outcome.IsSuccess());
		if (dry_run_outcome.GetError().GetErrorType() != Aws::EC2::EC2Errors::DRY_RUN_OPERATION) {
    		std::cout << "Failed dry run to start instance " << instance_id << ": "
        	<< dry_run_outcome.GetError().GetMessage() << std::endl;
    		return -1;
		}

		start_request.SetDryRun(false);
		auto start_instancesOutcome = ec2.StartInstances(start_request);

		if (!start_instancesOutcome.IsSuccess()) {
    		std::cout << "Failed to start instance " << instance_id << ": " <<
        	start_instancesOutcome.GetError().GetMessage() << std::endl;
		} else {
    		std::cout << "Successfully started instance " << instance_id <<
        	std::endl;
		}
	}
	Aws::ShutdownAPI(options);*/

	/*  // Stopping an instance

	Aws::SDKOptions options;
	options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;

	Aws::InitAPI(options);
	{
		Aws::String instance_id = "i-0742416418d041e07";
		
		Aws::Client::ClientConfiguration config;
		config.region = Aws::Region::AP_SOUTH_1;

		Aws::EC2::EC2Client ec2(Aws::Auth::AWSCredentials("AKIAITDKO3FURSYIACUQ", "Aq38PvhZVDwbinAx9GXnh07153SoL9gv9z4STHuM"), config);
		
		Aws::EC2::Model::StopInstancesRequest request;
		request.AddInstanceIds(instance_id);
		request.SetDryRun(true);

		auto dry_run_outcome = ec2.StopInstances(request);
		assert(!dry_run_outcome.IsSuccess());

		if (dry_run_outcome.GetError().GetErrorType() != Aws::EC2::EC2Errors::DRY_RUN_OPERATION) {
    		std::cout << "Failed dry run to stop instance " << instance_id << ": "
        	<< dry_run_outcome.GetError().GetMessage() << std::endl;
    		return -1;
		}

		request.SetDryRun(false);
		auto outcome = ec2.StopInstances(request);
		if (!outcome.IsSuccess()) {
    		std::cout << "Failed to stop instance " << instance_id << ": " <<
        	outcome.GetError().GetMessage() << std::endl;
		} else {
    		std::cout << "Successfully stopped instance " << instance_id <<
        	std::endl;
		}
	}		
	Aws::ShutdownAPI(options);*/
	

	/* // Creating an EC2 instance
	Aws::SDKOptions options;
	options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;

	Aws::InitAPI(options);
	{
		Aws::String ami_id = "ami-4fc58420";
		Aws::String instanceName = "myinstance1";
		
		Aws::Client::ClientConfiguration config;
		config.region = Aws::Region::AP_SOUTH_1;

		Aws::EC2::EC2Client ec2(Aws::Auth::AWSCredentials("AKIAITDKO3FURSYIACUQ", "Aq38PvhZVDwbinAx9GXnh07153SoL9gv9z4STHuM"), config);
		Aws::EC2::Model::RunInstancesRequest run_request;
		
		run_request.SetImageId(ami_id);
		run_request.SetInstanceType(Aws::EC2::Model::InstanceType::t2_micro);
		run_request.SetMinCount(1);
		run_request.SetMaxCount(1);

		auto run_outcome = ec2.RunInstances(run_request);
		if (!run_outcome.IsSuccess()) {
    		std::cout << "Failed to start ec2 instance " << instanceName <<
        	" based on ami " << ami_id << ":" <<
        	run_outcome.GetError().GetMessage() << std::endl;
    		return -1;
		}

		std::cout<<"CHECK 1"<<std::endl;

		const auto& instances = run_outcome.GetResult().GetInstances();
		if (instances.size() == 0) {
    		std::cout << "Failed to start ec2 instance " << instanceName <<
        	" based on ami " << ami_id << ":" <<
        	run_outcome.GetError().GetMessage() << std::endl;
    	return -1;
		} else {
			// Aws::EC2::Model::Instance
			std::cout<<"Image id: "<<instances[0].GetImageId()<<std::endl;
			std::cout<<"Instance id: "<<instances[0].GetInstanceId()<<std::endl;
			std::cout<<"Public IPv4: "<<instances[0].GetPublicIpAddress()<<std::endl;
		}		
	}
	Aws::ShutdownAPI(options); */


	// options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;   // Turning on basic logging
	

	/* // Basic Structure

	Aws::SDKOptions options;

	Aws::InitAPI(options);
	{
		// make your SDK calls here.
	}
	Aws::ShutdownAPI(options); */
	
	return 0;
}