/*
 *   All basic functions and primary connections to node.js
 */

#include <v8.h>
#include <node.h>
#include <doublefann.h>
#include <string.h>
#include "node-fann.h"

using namespace v8;
using namespace node;

Handle<Value> VException(const char *msg)
{
	HandleScope scope;
	return ThrowException(Exception::Error(String::New(msg)));
}

void NNet::PrototypeInit(Local<FunctionTemplate> t)
{
	t->InstanceTemplate()->SetInternalFieldCount(1);
	t->SetClassName(String::New("FANN"));
	NODE_SET_PROTOTYPE_METHOD(t, "train", Train);
	NODE_SET_PROTOTYPE_METHOD(t, "cascadetrain", CascadeTrain);
	NODE_SET_PROTOTYPE_METHOD(t, "train_once", TrainOnce);
	NODE_SET_PROTOTYPE_METHOD(t, "run", Run);

	// deprecated in favor of require('fann').get_...
	/*NODE_SET_PROTOTYPE_METHOD(t, "get_all_training_algorithms", GetTrainingAlgorithmList);
	NODE_SET_PROTOTYPE_METHOD(t, "get_all_activation_functions", GetActivationFunctionList);
	NODE_SET_PROTOTYPE_METHOD(t, "get_all_network_types", GetNetworkTypeList);
	NODE_SET_PROTOTYPE_METHOD(t, "get_all_stop_functions", GetStopFuncList);
	NODE_SET_PROTOTYPE_METHOD(t, "get_all_error_functions", GetErrorFuncList);*/

	NODE_SET_PROTOTYPE_METHOD(t, "activation_function", ActivationFunction);
	NODE_SET_PROTOTYPE_METHOD(t, "activation_function_hidden", ActivationFunctionHidden);
	NODE_SET_PROTOTYPE_METHOD(t, "activation_function_output", ActivationFunctionOutput);
	NODE_SET_PROTOTYPE_METHOD(t, "get_num_input", GetNumInput);
	NODE_SET_PROTOTYPE_METHOD(t, "get_num_output", GetNumOutput);
	NODE_SET_PROTOTYPE_METHOD(t, "get_total_neurons", GetTotalNeurons);
	NODE_SET_PROTOTYPE_METHOD(t, "get_total_connections", GetTotalConnections);
	NODE_SET_PROTOTYPE_METHOD(t, "get_network_type", GetNetworkType);
	NODE_SET_PROTOTYPE_METHOD(t, "get_connection_rate", GetConnectionRate);

// use net->layers instead !
//	NODE_SET_PROTOTYPE_METHOD(t, "get_num_layers", GetNumLayers);
//	NODE_SET_PROTOTYPE_METHOD(t, "get_layer_array", GetLayerArray);

	NODE_SET_PROTOTYPE_METHOD(t, "get_bias_array", GetBiasArray);
	NODE_SET_PROTOTYPE_METHOD(t, "get_weights", GetWeights);
	NODE_SET_PROTOTYPE_METHOD(t, "set_weights", SetWeights);
	t->InstanceTemplate()->SetAccessor(String::New("training_algorithm"), GetTrainingAlgorithm, SetTrainingAlgorithm);
	t->InstanceTemplate()->SetAccessor(String::New("learning_rate"), GetLearningRate, SetLearningRate);
	t->InstanceTemplate()->SetAccessor(String::New("learning_momentum"), GetLearningMomentum, SetLearningMomentum);
	t->InstanceTemplate()->SetAccessor(String::New("layers"), GetLayerArray);
}

void NNet::Initialize(Handle<Object> t)
{
	HandleScope scope;
	Local<FunctionTemplate> t1 = FunctionTemplate::New(NewStandard);
	Local<FunctionTemplate> t2 = FunctionTemplate::New(NewSparse);
	Local<FunctionTemplate> t3 = FunctionTemplate::New(NewShortcut);
//	Local<FunctionTemplate> t4 = FunctionTemplate::New(CloneNet);
	PrototypeInit(t1);
	PrototypeInit(t2);
	PrototypeInit(t3);
//	PrototypeInit(t4);
	t->Set(String::NewSymbol("standard"), t1->GetFunction());
	t->Set(String::NewSymbol("sparse"), t2->GetFunction());
	t->Set(String::NewSymbol("shortcut"), t3->GetFunction());
//	t->Set(String::NewSymbol("clone"), t4->GetFunction());

	NODE_SET_METHOD(t, "get_all_training_algorithms", GetTrainingAlgorithmList);
	NODE_SET_METHOD(t, "get_all_activation_functions", GetActivationFunctionList);
	NODE_SET_METHOD(t, "get_all_network_types", GetNetworkTypeList);
	NODE_SET_METHOD(t, "get_all_stop_functions", GetStopFuncList);
	NODE_SET_METHOD(t, "get_all_error_functions", GetErrorFuncList);
}

extern "C" void init (Handle<Object> target)
{
	HandleScope scope;
	NNet::Initialize(target);
}

