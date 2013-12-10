/*
 * Crud.h

 *
 *  Created on: Dec 2, 2013
 *      Author: kolundzija
 */

#ifndef CRUD_H_
#define CRUD_H_
#include <stddef.h>
using namespace std;

size_t writefunction(void *ptr, size_t size, size_t nmemb, void *stream);
size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);
class Crud {
public:
	Crud();
	virtual ~Crud();
	int createJob();
	void submitJob();
	void obtainingReference();
	void getUploadURL();
	//size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);
	void uploadContent();
	void downloadOutput();
	void getDataFileId();
	void obRef();
    //size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userp);
};

#endif /* CRUD_H_ */
