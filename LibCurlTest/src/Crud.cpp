/*
 * Crud.cpp
 *
 *  Created on: Dec 2, 2013
 *      Author: kolundzija
 */

#include "Crud.h"
#include <curl/curl.h>
#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#include  <cstring>

#include <fcntl.h>
#include <sys/stat.h>

CURL *curl;
CURLcode res;

Crud::Crud() {
	// TODO Auto-generated constructor stub

}

Crud::~Crud() {
	// TODO Auto-generated destructor stub
}

int Crud::createJob() {

	struct curl_slist *headers = NULL;

	std::string jobXML =
			"<job><name>Backwardstep test</name><software_name>TransAT 5.0</software_name> <executable_name>TransAT 5.0 run_without_UDF.sh</executable_name> <resource_name>Amazon EC2 CloudBroker GmbH CloudSME</resource_name> <region_name>Amazon EC2 CloudBroker GmbH CloudSME US East (Northern Virginia)</region_name> <instance_type_name>Amazon EC2 CloudBroker GmbH CloudSME Large - 64 bit</instance_type_name> <previous_job_id></previous_job_id> <nodes>1</nodes> <argument_string></argument_string> <stdout_file_name></stdout_file_name> <stderr_file_name></stderr_file_name> <start_immediately>true</start_immediately> <description>job created c++ libcurl</description></job>";

	curl = curl_easy_init();

	if (curl) {

		headers = curl_slist_append(headers, "Accept: */*");
		headers = curl_slist_append(headers, "Content-Type: application/xml");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, true);

		curl_easy_setopt(curl, CURLOPT_URL,
				"https://cloudsme-test.cloudbroker.com/jobs.xml");

		curl_easy_setopt(curl, CURLOPT_USERPWD, "kolundzija@ascomp.ch:ascomp");

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jobXML.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(jobXML.c_str()));

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			fprintf(stdout, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));

		curl_easy_cleanup(curl);
	}
	return 0;

}

// 1. getting credentials and URL to upload file
void Crud::getUploadURL() {

	curl = curl_easy_init();

	if (curl) {

		curl_easy_setopt(curl, CURLOPT_VERBOSE, true);

		curl_easy_setopt(curl, CURLOPT_URL,
				"https://cloudsme-test.cloudbroker.com/data_files/new/direct_file_upload.xml?job_id=264cf297-3bc7-42e1-8edc-5e2948ee62b6&data_file_name=backwardstep.grda");

		curl_easy_setopt(curl, CURLOPT_USERPWD, "kolundzija@ascomp.ch:ascomp");

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			fprintf(stdout, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));

		curl_easy_cleanup(curl);
	}

}

//2. uploading the content of file PUT
void Crud::uploadContent() {

	struct curl_slist *headers = NULL;
	FILE * hd_src;
	struct stat file_info;

	std::string file = "/home/kolundzija/tutorial/transat_mb.inp";

	stat(file.c_str(), &file_info);

	hd_src = fopen(file.c_str(), "rb");

	size_t size = (size_t) file_info.st_size;

	if (hd_src == NULL) {
		fputs("File error", stderr);
		exit(1);
	}

	curl = curl_easy_init();

	if (curl) {

		headers = curl_slist_append(headers,
				"Date:Mon, 09 Dec 2013 13:31:12 UTC");
		headers =
				curl_slist_append(headers,
						"Authorization:AWS AKIAJKXJUA54H6ZDPVRQ:8lznQLuTgUDCuA3ya9y8Pkh2viw=");
		//headers = curl_slist_append(headers, "Content-Length: 0");
		//headers = curl_slist_append(NULL, "Expect:");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, &read_callback);

		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_PUT, 1L);
		curl_easy_setopt(curl, CURLOPT_URL,
				"https://s3.amazonaws.com:443/cbp-0f094c9d-5380-498f-a4b3-813c098c43e1/264cf297-3bc7-42e1-8edc-5e2948ee62b6/backwardstep.bc");
		curl_easy_setopt(curl, CURLOPT_VERBOSE, true);
		curl_easy_setopt(curl, CURLOPT_INFILE, hd_src);

		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t )size);
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			fprintf(stdout, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));

		curl_easy_cleanup(curl);
	}

	fclose(hd_src);

}

size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream) {

	size_t retcode;
	curl_off_t nread;
	retcode = fread(ptr, size, nmemb, (FILE*) stream);
	nread = (curl_off_t) retcode;

	fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
	" bytes from file\n", nread);

	return retcode;

}

//3. obtaining the reference to the file for the platform PUT

//uses obref file and inside is description string since using PUT we cant send string just files
void Crud::obtainingReference() {

	struct curl_slist *headers = NULL;
	FILE * hd_src;
	struct stat file_info;

	std::string file = "/home/kolundzija/Desktop/obref.xml";

	stat(file.c_str(), &file_info);

	hd_src = fopen(file.c_str(), "rb");

	size_t size = (size_t) file_info.st_size;

	if (hd_src == NULL) {
		fputs("File error", stderr);
		exit(1);
	}

	curl = curl_easy_init();

	if (curl) {

		headers = curl_slist_append(headers, "Accept: */*");
		//headers = curl_slist_append(headers, "Content-Length: 100");
		headers = curl_slist_append(headers, "Content-Type: application/xml");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, &read_callback);

		curl_easy_setopt(curl, CURLOPT_UPLOAD, true);
		curl_easy_setopt(curl, CURLOPT_PUT, true);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, true);

		curl_easy_setopt(curl, CURLOPT_URL,
				"https://cloudsme-test.cloudbroker.com/data_files/new/link_upload.xml");

		curl_easy_setopt(curl, CURLOPT_USERPWD, "kolundzija@ascomp.ch:ascomp");

		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);;
		curl_easy_setopt(curl, CURLOPT_INFILE, hd_src);

		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t )size);
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			fprintf(stdout, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));

		curl_easy_cleanup(curl);
	}
	fclose(hd_src);

}

void Crud::getDataFileId() {
	curl = curl_easy_init();
	struct curl_slist *headers = NULL;
	if (curl) {

		curl = curl_easy_init();

		if (curl) {

			headers = curl_slist_append(headers, "Content-Length: 886");
			curl_easy_setopt(curl, CURLOPT_VERBOSE, true);

			curl_easy_setopt(curl, CURLOPT_URL,
					"https://cloudsme-test.cloudbroker.com/data_types");
			curl_easy_setopt(curl, CURLOPT_USERPWD,
					"kolundzija@ascomp.ch:ascomp");

			curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

			res = curl_easy_perform(curl);

			if (res != CURLE_OK)
				fprintf(stdout, "curl_easy_perform() failed: %s\n",
						curl_easy_strerror(res));

			curl_easy_cleanup(curl);
		}
	}
}

//submiting the job
void Crud::submitJob() {
	struct curl_slist *headers = NULL;
	FILE * hd_src;
	struct stat file_info;

	std::string file = "/home/kolundzija/Desktop/empty.xml";

	stat(file.c_str(), &file_info);

	hd_src = fopen(file.c_str(), "rb");

	size_t size = (size_t) file_info.st_size;

	if (hd_src == NULL) {
		fputs("File error", stderr);
		exit(1);
	}
	curl = curl_easy_init();

	if (curl) {
		headers = curl_slist_append(headers, "Content-Type: application/xml");
		headers = curl_slist_append(headers, "Accept: */*");

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, &read_callback);

		curl_easy_setopt(curl, CURLOPT_UPLOAD, true);
		curl_easy_setopt(curl, CURLOPT_PUT, true);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, true);

		curl_easy_setopt(curl, CURLOPT_URL,
				"https://cloudsme-test.cloudbroker.com/jobs/264cf297-3bc7-42e1-8edc-5e2948ee62b6/submit.xml");

		curl_easy_setopt(curl, CURLOPT_USERPWD, "kolundzija@ascomp.ch:ascomp");
		curl_easy_setopt(curl, CURLOPT_INFILE, hd_src);

		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t )size);
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			fprintf(stdout, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));

		curl_easy_cleanup(curl);

	}
}

void Crud::downloadOutput() {
	struct curl_slist *headers = NULL;
		FILE * hd_src;
		struct stat file_info;

		std::string file = "/home/kolundzija/Desktop/empty.xml";

		stat(file.c_str(), &file_info);

		hd_src = fopen(file.c_str(), "rb");

		size_t size = (size_t) file_info.st_size;

		if (hd_src == NULL) {
			fputs("File error", stderr);
			exit(1);
		}
		curl = curl_easy_init();

		if (curl) {
			headers = curl_slist_append(headers, "Content-Type: application/xml");
			headers = curl_slist_append(headers, "Accept: */*");

			curl_easy_setopt(curl, CURLOPT_READFUNCTION, &read_callback);

			curl_easy_setopt(curl, CURLOPT_UPLOAD, true);
			curl_easy_setopt(curl, CURLOPT_PUT, true);
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, true);

			curl_easy_setopt(curl, CURLOPT_URL,
					"https://cloudsme-test.cloudbroker.com/data_files/f6a57eea-2903-4047-847b-c5a3e9ba962c/download.xml");

			curl_easy_setopt(curl, CURLOPT_USERPWD, "kolundzija@ascomp.ch:ascomp");
			curl_easy_setopt(curl, CURLOPT_INFILE, hd_src);

			curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t )size);
			curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				fprintf(stdout, "curl_easy_perform() failed: %s\n",
						curl_easy_strerror(res));

			curl_easy_cleanup(curl);

		}


}

