/*
 *   All setters, getters and other information providers
 */

#include <v8.h>
#include <node.h>
#include <doublefann.h>
#include <string.h>
#include "fann.h"

using namespace v8;
using namespace node;

Handle<Value> NNet::NormalizeName(const char* origname, const char* prefix, int prefix_len)
{
	HandleScope scope;
	char algname[64];
	if (strncmp(origname, prefix, prefix_len) == 0) {
		origname +=	prefix_len;
	} 
	strncpy(algname, origname, 63);
	return scope.Close(String::New(origname));
}

Handle<Value> NNet::GetTrainingAlgorithm(Local<String> property, const AccessorInfo &info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);
	int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);
	enum fann_train_enum algo = fann_get_training_algorithm(net->FANN);

	if (algo >= 0 && algo < size) {
		return scope.Close(NormalizeName(FANN_TRAIN_NAMES[algo], TRAIN_PREFIX, TRAIN_PREFIX_LEN));
	} else {
		return Undefined();
	}
}

int NNet::_SeekCharArray(Local<String> value, const char* const* array, int size, const char* prefix)
{
	char name[64];
	char name2[128];
	value->WriteAscii(name, 0, 63);
	strcpy(name2, prefix);
	strcat(name2, name);
	for (int i=0; i<size; i++) {
		if (strcasecmp(name, array[i]) == 0 || strcasecmp(name2, array[i]) == 0) {
			return i;
		}
	}
	return -1;
}

void NNet::SetTrainingAlgorithm(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);
	int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);
	int num = -1;

	if (value->IsString()) {
		num = _SeekCharArray(String::Cast(*value), FANN_TRAIN_NAMES, size, TRAIN_PREFIX);
	} else if (value->IsNumber()) {
		num = value->NumberValue();
	}

	if (num >= 0 && num < size) {
		fann_set_training_algorithm(net->FANN, fann_train_enum(num));
	}
}

Handle<Value> NNet::GetTrainingAlgorithmList(const Arguments &args)
{
	HandleScope scope;
	int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);

	Local<Array> result_arr = Array::New(size);
	for (int i=0; i<size; i++) {
		result_arr->Set(i, NormalizeName(FANN_TRAIN_NAMES[i], TRAIN_PREFIX, TRAIN_PREFIX_LEN));
	}
	
	return scope.Close(result_arr);
}

Handle<Value> NNet::GetActivationFunctionList(const Arguments &args)
{
	HandleScope scope;
	int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);

	Local<Array> result_arr = Array::New(size);
	for (int i=0; i<size; i++) {
		result_arr->Set(i, NormalizeName(FANN_ACTIVATIONFUNC_NAMES[i], FANN_PREFIX, FANN_PREFIX_LEN));
	}
	
	return scope.Close(result_arr);
}

Handle<Value> NNet::GetLearningRate(Local<String> property, const AccessorInfo &info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);
	
	float rate = fann_get_learning_rate(net->FANN);
	return scope.Close(Number::New(rate));
}

Handle<Value> NNet::GetLearningMomentum(Local<String> property, const AccessorInfo &info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);
	
	float rate = fann_get_learning_momentum(net->FANN);
	return scope.Close(Number::New(rate));
}

void NNet::SetLearningRate(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);

	fann_set_learning_rate(net->FANN, value->NumberValue());
}

void NNet::SetLearningMomentum(Local<String> property, Local<Value> value, const AccessorInfo& info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);

	fann_set_learning_momentum(net->FANN, value->NumberValue());
}

Handle<Value> NNet::ActivationFunction(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	if (args.Length() < 2)
        return VException("Usage: func = activation_function(layer, neuron) or activation_function(layer, neuron, newfunc)");

	int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);
	int layer = args[0]->IntegerValue();
	int neuron = args[1]->IntegerValue();

	if (args.Length() >= 3) {
		int num = -1;
		if (args[2]->IsString()) {
			num = _SeekCharArray(String::Cast(*args[2]), FANN_ACTIVATIONFUNC_NAMES, size, FANN_PREFIX);
		} else if (args[2]->IsNumber()) {
			num = args[2]->NumberValue();
		}

		if (num >= 0 && num < size) {
			fann_set_activation_function(net->FANN, fann_activationfunc_enum(num), layer, neuron);
		}
	}

	enum fann_activationfunc_enum func = fann_get_activation_function(net->FANN, layer, neuron);
	if (func >= 0 && func < size) {
		return scope.Close(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, FANN_PREFIX_LEN));
	} else {
		return Undefined();
	}
}

Handle<Value> NNet::ActivationFunctionHidden(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);

	if (args.Length() >= 1) {
		int num = -1;
		if (args[0]->IsString()) {
			num = _SeekCharArray(String::Cast(*args[0]), FANN_ACTIVATIONFUNC_NAMES, size, FANN_PREFIX);
		} else if (args[0]->IsNumber()) {
			num = args[0]->NumberValue();
		}

		if (num >= 0 && num < size) {
			fann_set_activation_function_hidden(net->FANN, fann_activationfunc_enum(num));
		}
	}

	enum fann_activationfunc_enum func = fann_get_activation_function(net->FANN, 1, 0);
	if (func >= 0 && func < size) {
		return scope.Close(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, FANN_PREFIX_LEN));
	} else {
		return Undefined();
	}
}

Handle<Value> NNet::ActivationFunctionOutput(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	int size = sizeof(FANN_ACTIVATIONFUNC_NAMES)/sizeof(char*);

	if (args.Length() >= 1) {
		int num = -1;
		if (args[0]->IsString()) {
			num = _SeekCharArray(String::Cast(*args[0]), FANN_ACTIVATIONFUNC_NAMES, size, FANN_PREFIX);
		} else if (args[0]->IsNumber()) {
			num = args[0]->NumberValue();
		}

		if (num >= 0 && num < size) {
			fann_set_activation_function_output(net->FANN, fann_activationfunc_enum(num));
		}
	}

	enum fann_activationfunc_enum func = fann_get_activation_function(net->FANN, fann_get_num_layers(net->FANN)-1, 0);
	if (func >= 0 && func < size) {
		return scope.Close(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, FANN_PREFIX_LEN));
	} else {
		return Undefined();
	}
}

