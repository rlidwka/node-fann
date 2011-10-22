/*
 *   Returns constant lists
 */

#include <v8.h>
#include <node.h>
#include <doublefann.h>
#include "node-futil.h"

using namespace v8;
using namespace node;

Handle<Value> GetTrainingAlgorithmList(const Arguments &args)
{
	HandleScope scope;
	int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);

	Local<Array> result_arr = Array::New(size);
	for (int i=0; i<size; i++) {
		result_arr->Set(i, NormalizeName(FANN_TRAIN_NAMES[i], TRAIN_PREFIX, sizeof(TRAIN_PREFIX)-1));
	}
	
	return scope.Close(result_arr);
}

Handle<Value> GetActivationFunctionList(const Arguments &args)
{
	HandleScope scope;
	int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);

	Local<Array> result_arr = Array::New(size);
	for (int i=0; i<size; i++) {
		result_arr->Set(i, NormalizeName(FANN_ACTIVATIONFUNC_NAMES[i], FANN_PREFIX, sizeof(FANN_PREFIX)-1));
	}
	
	return scope.Close(result_arr);
}

Handle<Value> GetNetworkTypeList(const Arguments &args)
{
	HandleScope scope;
	int size = sizeof(FANN_NETTYPE_NAMES)/sizeof(char*);

	Local<Array> result_arr = Array::New(size);
	for (int i=0; i<size; i++) {
		result_arr->Set(i, NormalizeName(FANN_NETTYPE_NAMES[i], NETTYPE_PREFIX, sizeof(NETTYPE_PREFIX)-1));
	}
	
	return scope.Close(result_arr);
}

Handle<Value> GetErrorFuncList(const Arguments &args)
{
	HandleScope scope;
	int size = sizeof(FANN_ERRORFUNC_NAMES)/sizeof(char*);

	Local<Array> result_arr = Array::New(size);
	for (int i=0; i<size; i++) {
		result_arr->Set(i, NormalizeName(FANN_ERRORFUNC_NAMES[i], ERRORFUNC_PREFIX, sizeof(ERRORFUNC_PREFIX)-1));
	}
	
	return scope.Close(result_arr);
}

Handle<Value> GetStopFuncList(const Arguments &args)
{
	HandleScope scope;
	int size = sizeof(FANN_STOPFUNC_NAMES)/sizeof(char*);

	Local<Array> result_arr = Array::New(size);
	for (int i=0; i<size; i++) {
		result_arr->Set(i, NormalizeName(FANN_STOPFUNC_NAMES[i], STOPFUNC_PREFIX, sizeof(STOPFUNC_PREFIX)-1));
	}
	
	return scope.Close(result_arr);
}


