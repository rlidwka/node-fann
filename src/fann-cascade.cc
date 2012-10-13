/*
 *   All functions related to cascade training
 */

#include "node-fann.h"

Handle<Value> NNet::CascadeTrainOnData(struct fann_train_data *traindata, unsigned int max_neurons, unsigned int neurons_between_reports, float desired_error)
{
	fann_cascadetrain_on_data(FANN, traindata, max_neurons, neurons_between_reports, desired_error);
	return Undefined();
}

Handle<Value> NNet::CascadeTrain(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	struct fann_train_data *traindata = NULL;
	net->MakeTrainData(args, &traindata);
	if (traindata == NULL)
        return VException("Internal error");

	unsigned int max_neurons = 100000;
	unsigned int neurons_between_reports = 1000;
	float desired_error = 0.001;
	int scale = 0;
	if (args.Length() >= 2) {
		Local<Object> params = *args[1]->ToObject();
		if (params->Has(String::New("neurons"))) {
			max_neurons = params->Get(String::New("neurons"))->IntegerValue();
		}
		if (params->Has(String::New("neurons_between_reports"))) {
			neurons_between_reports = params->Get(String::New("neurons_between_reports"))->IntegerValue();
		}
		if (params->Has(String::New("error"))) {
			desired_error = params->Get(String::New("error"))->NumberValue();
		}
		if (params->Has(String::New("scale"))) {
			scale = params->Get(String::New("scale"))->BooleanValue();
		}
	}
	if (scale) {
		fann_scale_train(net->FANN, traindata);
		net->scale_present = true;
	}
	net->CascadeTrainOnData(traindata, max_neurons, neurons_between_reports, desired_error);
	fann_destroy_train(traindata);
	return Undefined();
}

