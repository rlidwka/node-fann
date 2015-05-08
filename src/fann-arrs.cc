/*
 *   Returns constant lists
 */

#include "node-futil.h"

NAN_METHOD(GetTrainingAlgorithmList)
{
  NanScope();
  int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);

  Local<Array> result_arr = NanNew<Array>(size);
  for (int i=0; i<size; i++) {
    result_arr->Set(i, NormalizeName(FANN_TRAIN_NAMES[i], TRAIN_PREFIX, sizeof(TRAIN_PREFIX)-1));
  }

  NanReturnValue(result_arr);
}

NAN_METHOD(GetActivationFunctionList)
{
  NanScope();
  int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);

  Local<Array> result_arr = NanNew<Array>(size);
  for (int i=0; i<size; i++) {
    result_arr->Set(i, NormalizeName(FANN_ACTIVATIONFUNC_NAMES[i], FANN_PREFIX, sizeof(FANN_PREFIX)-1));
  }

  NanReturnValue(result_arr);
}

NAN_METHOD(GetNetworkTypeList)
{
  NanScope();
  int size = sizeof(FANN_NETTYPE_NAMES)/sizeof(char*);

  Local<Array> result_arr = NanNew<Array>(size);
  for (int i=0; i<size; i++) {
    result_arr->Set(i, NormalizeName(FANN_NETTYPE_NAMES[i], NETTYPE_PREFIX, sizeof(NETTYPE_PREFIX)-1));
  }

  NanReturnValue(result_arr);
}

NAN_METHOD(GetErrorFuncList)
{
  NanScope();
  int size = sizeof(FANN_ERRORFUNC_NAMES)/sizeof(char*);

  Local<Array> result_arr = NanNew<Array>(size);
  for (int i=0; i<size; i++) {
    result_arr->Set(i, NormalizeName(FANN_ERRORFUNC_NAMES[i], ERRORFUNC_PREFIX, sizeof(ERRORFUNC_PREFIX)-1));
  }

  NanReturnValue(result_arr);
}

NAN_METHOD(GetStopFuncList)
{
  NanScope();
  int size = sizeof(FANN_STOPFUNC_NAMES)/sizeof(char*);

  Local<Array> result_arr = NanNew<Array>(size);
  for (int i=0; i<size; i++) {
    result_arr->Set(i, NormalizeName(FANN_STOPFUNC_NAMES[i], STOPFUNC_PREFIX, sizeof(STOPFUNC_PREFIX)-1));
  }

  NanReturnValue(result_arr);
}

