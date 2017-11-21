#ifndef _EC2_H
#define _EC2_H

#include <vector>

#define MAX_KEY_SIZE 64
#define MAX_REGION_SIZE 16
#define MAX_ID_SIZE 32
#define MAX_IPV4_SIZE 16

struct myAccount {
	int total, stopped, others;
	char key1[MAX_KEY_SIZE];
	char key2[MAX_KEY_SIZE];
	char region[MAX_REGION_SIZE];
};

struct Instance {
	int state;
	char id[MAX_ID_SIZE];
	char ip[MAX_IPV4_SIZE];
};

typedef struct myAccount myAccount;
typedef struct Instances Instances;

myAccount* account(const char* access_key_id, const char* secret_access_key, const char* region);
std::vector<Instance*> getInstances(myAccount* acc);
void startInstances(myAccount* acc, std::vector<Instance*>& ins, int n);
void createInstances(myAccount* acc, std::vector<Instance*>& ins, int n);

#endif