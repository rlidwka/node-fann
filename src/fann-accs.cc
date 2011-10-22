/*
 *   All setters, getters and other information providers
 */

#include <v8.h>
#include <node.h>
#include <doublefann.h>
#include <string.h>
#include <ctype.h>
#include "node-fann.h"

using namespace v8;
using namespace node;

Handle<Value> NNet::GetTrainingAlgorithm(Local<String> property, const AccessorInfo &info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);
	int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);
	enum fann_train_enum algo = fann_get_training_algorithm(net->FANN);

	if (algo >= 0 && algo < size) {
		return scope.Close(NormalizeName(FANN_TRAIN_NAMES[algo], TRAIN_PREFIX, sizeof(TRAIN_PREFIX)-1));
	} else {
		return Undefined();
	}
}

/*Handle<Value> NNet::GetNetworkType(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	int size = sizeof(FANN_TRAIN_NAMES)/sizeof(char*);
	enum fann_train_enum algo = fann_get_training_algorithm(net->FANN);

	if (algo >= 0 && algo < size) {
		return scope.Close(NormalizeName(FANN_TRAIN_NAMES[algo], TRAIN_PREFIX, sizeof(TRAIN_PREFIX)-1));
	} else {
		return Undefined();
	}
}*/

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
		return scope.Close(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, sizeof(FANN_PREFIX)-1));
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
		return scope.Close(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, sizeof(FANN_PREFIX)-1));
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
		return scope.Close(NormalizeName(FANN_ACTIVATIONFUNC_NAMES[func], FANN_PREFIX, sizeof(FANN_PREFIX)-1));
	} else {
		return Undefined();
	}
}

Handle<Value> NNet::GetNumInput(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

	unsigned int ret = fann_get_num_input(net->FANN);
	return scope.Close(Integer::New(ret));
}

Handle<Value> NNet::GetNumOutput(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

	unsigned int ret = fann_get_num_output(net->FANN);
	return scope.Close(Integer::New(ret));
}

Handle<Value> NNet::GetTotalNeurons(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

	unsigned int ret = fann_get_total_neurons(net->FANN);
	return scope.Close(Integer::New(ret));
}

Handle<Value> NNet::GetTotalConnections(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

	unsigned int ret = fann_get_total_connections(net->FANN);
	return scope.Close(Integer::New(ret));
}

