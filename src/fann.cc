/*
 *   All basic functions and primary connections to node.js
 */

#include <string.h>
#include "node-fann.h"

void NNet::PrototypeInit(Local<FunctionTemplate> t)
{
  t->InstanceTemplate()->SetInternalFieldCount(1);
  t->SetClassName(NanNew<String>("FANN"));
  NODE_SET_PROTOTYPE_METHOD(t, "train", Train);
  NODE_SET_PROTOTYPE_METHOD(t, "cascadetrain", CascadeTrain);
  NODE_SET_PROTOTYPE_METHOD(t, "train_once", TrainOnce);
  NODE_SET_PROTOTYPE_METHOD(t, "run", Run);
  NODE_SET_PROTOTYPE_METHOD(t, "save", SaveToFile);

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
  NODE_SET_PROTOTYPE_METHOD(t, "get_MSE", GetMse);
  NODE_SET_PROTOTYPE_METHOD(t, "get_num_output", GetNumOutput);
  NODE_SET_PROTOTYPE_METHOD(t, "get_total_neurons", GetTotalNeurons);
  NODE_SET_PROTOTYPE_METHOD(t, "get_total_connections", GetTotalConnections);
  NODE_SET_PROTOTYPE_METHOD(t, "get_network_type", GetNetworkType);
  NODE_SET_PROTOTYPE_METHOD(t, "get_connection_rate", GetConnectionRate);

  // use net->layers instead ?
  NODE_SET_PROTOTYPE_METHOD(t, "get_num_layers", GetNumLayers);
  NODE_SET_PROTOTYPE_METHOD(t, "get_layer_array", GetLayerArray);

  NODE_SET_PROTOTYPE_METHOD(t, "get_bias_array", GetBiasArray);
  NODE_SET_PROTOTYPE_METHOD(t, "get_weight_array", GetWeights);
  NODE_SET_PROTOTYPE_METHOD(t, "set_weight_array", SetWeights);
  NODE_SET_PROTOTYPE_METHOD(t, "get_weight", GetWeights);
  NODE_SET_PROTOTYPE_METHOD(t, "set_weight", SetWeights);
  t->InstanceTemplate()->SetAccessor(NanNew<String>("training_algorithm"), GetTrainingAlgorithm, SetTrainingAlgorithm);
  t->InstanceTemplate()->SetAccessor(NanNew<String>("learning_rate"), GetLearningRate, SetLearningRate);
  t->InstanceTemplate()->SetAccessor(NanNew<String>("learning_momentum"), GetLearningMomentum, SetLearningMomentum);
  t->InstanceTemplate()->SetAccessor(NanNew<String>("layers"), GetLayerArray);
}

void NNet::Initialize(Handle<Object> t)
{
  NanScope();
  Local<FunctionTemplate> t1 = NanNew<FunctionTemplate>(NewStandard);
  Local<FunctionTemplate> t2 = NanNew<FunctionTemplate>(NewSparse);
  Local<FunctionTemplate> t3 = NanNew<FunctionTemplate>(NewShortcut);
  Local<FunctionTemplate> t4 = NanNew<FunctionTemplate>(NewFromFile);
//  Local<FunctionTemplate> t5 = NanNew<FunctionTemplate>(CloneNet);
  PrototypeInit(t1);
  PrototypeInit(t2);
  PrototypeInit(t3);
  PrototypeInit(t4);
  t->Set(NanNew<String>("standard"), t1->GetFunction());
  t->Set(NanNew<String>("sparse"), t2->GetFunction());
  t->Set(NanNew<String>("shortcut"), t3->GetFunction());
  t->Set(NanNew<String>("load"), t4->GetFunction());
//  t->Set(String::NewSymbol("clone"), t4->GetFunction());

  NODE_SET_METHOD(t, "get_all_training_algorithms", GetTrainingAlgorithmList);
  NODE_SET_METHOD(t, "get_all_activation_functions", GetActivationFunctionList);
  NODE_SET_METHOD(t, "get_all_network_types", GetNetworkTypeList);
  NODE_SET_METHOD(t, "get_all_stop_functions", GetStopFuncList);
  NODE_SET_METHOD(t, "get_all_error_functions", GetErrorFuncList);
}

extern "C" void init (Handle<Object> target)
{
  NanScope();
  NNet::Initialize(target);
}

#ifdef NODE_MODULE
NODE_MODULE(fann, init)
#endif
