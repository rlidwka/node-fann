/*
 *   All basic functions and primary connections to node.js
 */

#include <v8.h>
#include <node.h>
#include <doublefann.h>
#include <string.h>
#include "fann.h"

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
	NODE_SET_PROTOTYPE_METHOD(t, "train", Train);
	NODE_SET_PROTOTYPE_METHOD(t, "train_once", TrainOnce);
	NODE_SET_PROTOTYPE_METHOD(t, "run", Run);
	NODE_SET_PROTOTYPE_METHOD(t, "get_training_algorithms", GetTrainingAlgorithmList);
	NODE_SET_PROTOTYPE_METHOD(t, "get_activation_functions", GetActivationFunctionList);
	NODE_SET_PROTOTYPE_METHOD(t, "activation_function", ActivationFunction);
	NODE_SET_PROTOTYPE_METHOD(t, "activation_function_hidden", ActivationFunctionHidden);
	NODE_SET_PROTOTYPE_METHOD(t, "activation_function_output", ActivationFunctionOutput);
	NODE_SET_PROTOTYPE_METHOD(t, "get_num_input", GetNumInput);
	NODE_SET_PROTOTYPE_METHOD(t, "get_num_output", GetNumOutput);
	NODE_SET_PROTOTYPE_METHOD(t, "get_total_neurons", GetTotalNeurons);
	NODE_SET_PROTOTYPE_METHOD(t, "get_total_connections", GetTotalConnections);
	t->InstanceTemplate()->SetAccessor(String::New("training_algorithm"), GetTrainingAlgorithm, SetTrainingAlgorithm);
	t->InstanceTemplate()->SetAccessor(String::New("learning_rate"), GetLearningRate, SetLearningRate);
	t->InstanceTemplate()->SetAccessor(String::New("learning_momentum"), GetLearningMomentum, SetLearningMomentum);
}

void NNet::Initialize(Handle<Object> target)
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
	target->Set(String::NewSymbol("standard"), t1->GetFunction());
	target->Set(String::NewSymbol("shortcut"), t2->GetFunction());
	target->Set(String::NewSymbol("sparse"), t3->GetFunction());
//	target->Set(String::NewSymbol("clone"), t4->GetFunction());
	target->Set(String::NewSymbol("get_training_algorithms"), FunctionTemplate::New(GetTrainingAlgorithmList)->GetFunction());
	target->Set(String::NewSymbol("get_activation_functions"), FunctionTemplate::New(GetActivationFunctionList)->GetFunction());
}

extern "C" void init (Handle<Object> target)
{
	HandleScope scope;
	NNet::Initialize(target);
}

