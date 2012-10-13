/*
 *   All setters, getters and other information providers
 */

#include <string.h>
#include <stdio.h>
#include "node-fann.h"

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

Handle<Value> NNet::GetNetworkType(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	int size = sizeof(FANN_NETTYPE_NAMES)/sizeof(char*);
	enum fann_nettype_enum ret = fann_get_network_type(net->FANN);

	if (ret >= 0 && ret < size) {
		return scope.Close(NormalizeName(FANN_NETTYPE_NAMES[ret], NETTYPE_PREFIX, sizeof(NETTYPE_PREFIX)-1));
	} else {
		return Undefined();
	}
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

Handle<Value> NNet::GetConnectionRate(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

	float ret = fann_get_connection_rate(net->FANN);
	return scope.Close(Number::New(ret));
}

Handle<Value> NNet::GetNumLayers(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());

	unsigned int ret = fann_get_num_layers(net->FANN);
	return scope.Close(Integer::New(ret));
}

Handle<Value> NNet::GetLayerArray(Local<String> property, const AccessorInfo &info)
{
	HandleScope scope;
	Local<Object> self = info.Holder();
	NNet *net = ObjectWrap::Unwrap<NNet>(self);
	return scope.Close(net->GetLayers());
}

Handle<Value> NNet::GetLayerArray(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	return scope.Close(net->GetLayers());
}
	
Handle<Value> NNet::GetLayers()
{
	HandleScope scope;
	int size = fann_get_num_layers(FANN);
	unsigned int* layers = new unsigned int[size];
	fann_get_layer_array(FANN, layers);
	
	Local<Array> result_arr = Array::New();
	for (int i=0; i<size; i++) {
		result_arr->Set(i, Number::New(layers[i]));
	}

	delete[] layers;
	return scope.Close(result_arr);
}

Handle<Value> NNet::GetBiasArray(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	
	int size = fann_get_num_layers(net->FANN);
	unsigned int* layers = new unsigned int[size];
	fann_get_bias_array(net->FANN, layers);
	
	Local<Array> result_arr = Array::New();
	for (int i=0; i<size; i++) {
		result_arr->Set(i, Number::New(layers[i]));
	}

	delete[] layers;
	return scope.Close(result_arr);
}

Handle<Value> NNet::GetWeights(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	int size = fann_get_total_connections(net->FANN);
	struct fann_connection *conns = new struct fann_connection[size];
	fann_get_connection_array(net->FANN, conns);
	
	Local<Object> result_object = Object::New();
	for (int i=0; i<size; i++) {
		Local<Object> obj;
		if (!result_object->Has(conns[i].from_neuron)) {
			obj = Object::New();
			result_object->Set(conns[i].from_neuron, obj);
		} else {
			obj = Object::Cast(*result_object->Get(conns[i].from_neuron));
		}
		obj->Set(conns[i].to_neuron, Number::New(conns[i].weight));
	}

	delete[] conns;
	return scope.Close(result_object);
}

Handle<Value> NNet::SetWeightsArr(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	
	if (!args[0]->IsObject())
        return VException("First argument should be object");
	Local<Array> arg = Array::Cast(*args[0]);
	Local<Array> keys = arg->GetOwnPropertyNames();

	struct fann_connection *conns = new struct fann_connection[fann_get_total_connections(net->FANN)];
	int counter = 0;
	for (int i=0; i<keys->Length(); i++) {
		Local<Value> idx = keys->Get(i);
		if (!arg->Get(idx)->IsObject()) continue;
		Local<Object> obj = Object::Cast(*arg->Get(idx));
		Local<Array> keys2 = obj->GetOwnPropertyNames();
		for (int j=0; j<keys2->Length(); j++) {
			conns[counter].from_neuron = idx->IntegerValue();
			conns[counter].to_neuron = keys2->Get(j)->IntegerValue();
			conns[counter].weight = obj->Get(keys2->Get(j))->NumberValue();
			counter++;
		}
	}
	fann_set_weight_array(net->FANN, conns, counter);

	delete[] conns;
	return Undefined();
}

Handle<Value> NNet::SetWeights(const Arguments &args)
{
	HandleScope scope;
	if (args[0]->IsObject())
		return SetWeightsArr(args);
	
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	if (args.Length() < 3)
        return VException("Usage: set_weights(new_object) or set_weight(from_neuron, to_neuron, weight)");

	unsigned int from_neuron = args[0]->IntegerValue();
	unsigned int to_neuron = args[1]->IntegerValue();
	fann_type weight = args[2]->NumberValue();

	fann_set_weight(net->FANN, from_neuron, to_neuron, weight);
	return Undefined();
}

