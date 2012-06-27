/*
 * logger.h
 *
 *  Created on: 26-04-2012
 *      Author: ptab
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdlib.h>

#define CHECK(x, msg) if (!(x)) { fprintf(stderr, "%s:%d Assertion failed: %s; %s\n", __FILE__, __LINE__, #x, msg); exit(1); };

#define LOG0(str) { printf(str "\n"); fflush(stderr); }
#define LOG1(str, x1) { printf(str "\n", x1); fflush(stderr); }
#define LOG2(str, x1, x2) { printf(str "\n", x1, x2); fflush(stderr); }
#define LOG3(str, x1, x2, x3) { printf(str "\n", x1, x2, x3); fflush(stderr); }
#define LOG4(str, x1, x2, x3, x4) { printf(str "\n", x1, x2, x3, x4); fflush(stderr); }

#endif /* LOGGER_H_ */
