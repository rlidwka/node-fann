/*
 *  This contains basic declarations, should be included everywhere one way or another
 */

#include <v8.h>
#include <node.h>
#include <doublefann.h>
#include <nan.h>

using namespace v8;
using namespace node;

const char TRAIN_PREFIX[]      = "FANN_TRAIN_";
const char FANN_PREFIX[]       = "FANN_";
const char NETTYPE_PREFIX[]    = "FANN_NETTYPE_";
const char STOPFUNC_PREFIX[]   = "FANN_STOPFUNC_";
const char ERRORFUNC_PREFIX[]  = "FANN_ERRORFUNC_";

Handle<Value> NormalizeName(const char* origname, const char* prefix, int prefix_len);
int _SeekCharArray(Local<String> value, const char* const* array, int size, const char* prefix);
char* strncpy_lower(char *dest, const char *src, size_t n);

NAN_METHOD(GetTrainingAlgorithmList);
NAN_METHOD(GetActivationFunctionList);
NAN_METHOD(GetErrorFuncList);
NAN_METHOD(GetStopFuncList);
NAN_METHOD(GetNetworkTypeList);

