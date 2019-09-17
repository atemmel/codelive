#pragma once
#include <fcntl.h>
#include <unistd.h>
#include <string>

void redirect(const std::string &call, const std::string &fout, const std::string &ferr) {
	printf("Attempting to execute: %s", call.c_str() );
	int out = open(fout.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0600);
	int err = open(ferr.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if(-1 == err || -1 == out) { 
		perror("Cannot open descriptors"); 
		return; 
	}

	int save_out = dup(fileno(stdout));
 	int save_err = dup(fileno(stderr));

	if(-1 == dup2(err, fileno(stderr))) { 
		perror("Cannot redirect stderr"); 
		return; 
	}
	if(-1 == dup2(err, fileno(stdout))) {
		perror("Cannot redirect stdout");
		return;
	}

	system(call.c_str());

	fflush(stdout); close(out);
	fflush(stderr); close(err);

	dup2(save_out, fileno(stdout));
	dup2(save_err, fileno(stderr));

	close(save_out);
	close(save_err);
}
