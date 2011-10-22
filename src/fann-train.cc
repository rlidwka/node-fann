/*
 *   All functions related to training a network
 */

#include <v8.h>
#include <node.h>
#include <doublefann.h>
#include <string.h>
#include "node-fann.h"

using namespace v8;
using namespace node;

fann_type *dataset_raw;
void FANN_API _TrainCB_FromRAW(unsigned int num, unsigned int innum, unsigned int outnum, fann_type *in, fann_type *out) {
	for (int c=0; c<innum; c++) {
		in[c] = dataset_raw[num*(innum+outnum) + c];
	}
	for (int c=0; c<outnum; c++) {
		out[c] = dataset_raw[num*(innum+outnum) + innum + c];
	}
}

Handle<Value> NNet::MakeTrainData(const Arguments &args, struct fann_train_data **traindata)
{
	if (args.Length() < 1)
        return VException("No arguments supplied");

	if (!args[0]->IsArray())
        return VException("First argument should be 2d-array (training data set)");
		
	int input_num = fann_get_num_input(FANN);
	int output_num = fann_get_num_output(FANN);
	
	Local<Array> dataset = Array::Cast(*args[0]->ToObject());
	dataset_raw = new fann_type[(input_num+output_num) * dataset->Length()];
	fann_type *dataset_raw_ptr = dataset_raw;
	//fann_set_user_data(net->FANN, (void*)dataset_raw);
	for (int i=0; i<dataset->Length(); i++) {
		Local<Value> t = dataset->Get(i);
		if (!t->IsArray()) {
			return VException("First argument should be 2d-array (training data set)");
		}
		Local<Array> data = Array::Cast(*t);
		if (data->Length() != 2) {
			return VException("Incorrect dataset");
		}
		Local<Value> tin = data->Get(0);
		Local<Value> tout = data->Get(1);
		if (!tin->IsArray() || !tout->IsArray()) {
			return VException("Incorrect dataset");
		}

		Local<Array> datain = Array::Cast(*tin);
		Local<Array> dataout = Array::Cast(*tout);
		if (datain->Length() != input_num) {
			return VException("Incorrect dataset (input length doesn't match input layer)");
		}
		if (dataout->Length() != output_num) {
			return VException("Incorrect dataset (output length doesn't match output layer)");
		}
		for (int c=0; c<input_num; c++) {
			*dataset_raw_ptr = datain->Get(c)->NumberValue();
			dataset_raw_ptr++;
		}
		for (int c=0; c<output_num; c++) {
			*dataset_raw_ptr = dataout->Get(c)->NumberValue();
			dataset_raw_ptr++;
		}
	}

	*traindata = fann_create_train_from_callback(dataset->Length(), input_num, output_num, _TrainCB_FromRAW);
	delete[] dataset_raw;
	
	return Undefined();
}

Handle<Value> NNet::TrainOnData(struct fann_train_data *traindata, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error)
{
	fann_train_on_data(FANN, traindata, max_epochs, epochs_between_reports, desired_error);
	return Undefined();
}

Handle<Value> NNet::Train(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	struct fann_train_data *traindata = NULL;
	net->MakeTrainData(args, &traindata);
	if (traindata == NULL)
        return VException("Internal error");

	unsigned int max_epochs = 100000;
	unsigned int epochs_between_reports = 1000;
	float desired_error = 0.001;
	int scale = 0;
	if (args.Length() >= 2) {
		Local<Object> params = *args[1]->ToObject();
		if (params->Has(String::New("epochs"))) {
			max_epochs = params->Get(String::New("epochs"))->IntegerValue();
		}
		if (params->Has(String::New("epochs_between_reports"))) {
			epochs_between_reports = params->Get(String::New("epochs_between_reports"))->IntegerValue();
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
	net->TrainOnData(traindata, max_epochs, epochs_between_reports, desired_error);
	fann_destroy_train(traindata);
	return Undefined();
}

Handle<Value> NNet::TrainOnce(const Arguments &args)
{
	HandleScope scope;
	NNet *net = ObjectWrap::Unwrap<NNet>(args.This());
	if (args.Length() < 2)
        return VException("2 arguments needed");

	if (!args[0]->IsArray())
        return VException("First argument should be array (training input)");
	if (!args[1]->IsArray())
        return VException("Second argument should be array (training output)");
	
	Local<Array> datain = Array::Cast(*args[0]->ToObject());
	Local<Array> dataout = Array::Cast(*args[1]->ToObject());
	fann_type *dataset_in = new fann_type[datain->Length()];
	fann_type *dataset_out = new fann_type[dataout->Length()];
	for (int i=0; i<datain->Length(); i++) {
		dataset_in[i] = datain->Get(i)->NumberValue();
	}
	for (int i=0; i<dataout->Length(); i++) {
		dataset_out[i] = dataout->Get(i)->NumberValue();
	}

	fann_train(net->FANN, dataset_in, dataset_out);

	delete[] dataset_in;
	delete[] dataset_out;
	return Undefined();
}

