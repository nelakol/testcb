//============================================================================
// Name        : LibCurlTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <string.h>
#include "Crud.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include  <cstring>



int main() {

	Crud *c = new Crud();



	//c->createJob();

	//c-> getUploadURL();

 //c->uploadContent();
//c->obtainingReference();
	//c->getDataFileId();


//c-> submitJob();
c-> downloadOutput();

	return 0;
}

