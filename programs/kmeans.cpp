/*
 * main.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "../src/common.h"
#include "../src/Partition.hpp"

#include "../src/KMAlgo.hpp"
#include "../src/KMInstance.hpp"
#include "../src/KMConstraints.hpp"

#include "../src/Env.hpp"
#include "../src/Number.hpp"

int main(int argc, char ** argv) {
	std::string const dataFileName(argv[1]);
	size_t const k(atoi(argv[2]));

	KMInstance instance;
	instance.readData(dataFileName);
	Agregations agregations;
	instance.mustLink(0, 1);
	instance.mustLink(0, 2);
	instance.mustLink(0, 3);
	instance.buildMustLink(agregations);
	KMAlgo kmeans(instance, k);
//	Number::SetSeed(argc > 3 ? atoi(argv[3]) : 0);
//	kmeans.random();
//	kmeans.run(600);

//	kmeans.run2();
//	OUT<< data;
//	if (argc != 3) {
//		std::cout << "kmeans <graphFile> <k>\n";
//		return 0;
//	}
//	std::string dataFileName(argv[1]);
//	std::string partitionName(argv[2]);
//
//	Partition partition;
//	IBuilder::Get<IPartition>(partition, partitionName);
	return 0;
}