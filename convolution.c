#include "convolution.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "logging.h"

//add padding to blob
BLOB* pad(BLOB* in, int pad){

    //create output blob
    BLOB* out = blob_calloc(in->d, in->h+2*pad, in->w+pad*2);

    //copy non-padded input into output blob
    int tx = 4;
            
    for(int z=0;z<in->d;z++)
    for(int xx=0;xx<in->w; xx+=tx)    
       for(int y=0;y<in->h;y++)
          for(int x=xx;x<xx+tx;x++)
              blob_data(out,z,y+pad,x+pad)= blob_data(in,z,y,x);

    //return pointer to padded blob
    return out;
}


BLOB* load_weights(BLOB* b, conv_param_t* p){

    //open weights file for reading
    FILE* fp = fopen(p->weights, "rb");
    if(fp==NULL)
        error("could not open file %s for reading\n",p->weights);

    //for fully connected layers the kernel size is equal to the input size
    int Ky=(p->fc)?b->h:p->Ky;
    int Kx=(p->fc)?b->w:p->Kx;

    //allocate 3D blob, and emulate 4D in KxKy later
    BLOB* w=blob_alloc(p->num_out, b->d/p->group, Ky*Kx);

    //fill 4D weight structure
    for(int g=0;g<p->group;g++)
        for(int o=g*(p->num_out/p->group);o<(g+1)*(p->num_out/p->group);o++)
            for(int i=g*(b->d/p->group);i<(g+1)*(b->d/p->group);i++)
                //note: each output map has only  b->d/p->group input maps. Hence the absolute index of i is subtracted when storing in w!
                if((int)fread( &(blob_data(w,o,i-g*(b->d/p->group),0)),sizeof(float),Ky*Kx, fp)!=Ky*Kx)
                    error("loading weights from file %s\n", p->weights);

    //close file
    fclose(fp);

    //return weight blob
    return w;
}

float* load_1d(const char* fname, size_t num){

    //open file for reading
    FILE* fp = fopen(fname, "rb");
    if(fp==NULL)
        error("could not open file %s for reading\n",fname);

    //read in array
    float* arr= (float*) malloc(sizeof(float)*num);
    if(fread(arr,sizeof(float), num, fp)!=num)
        error("loading data from file %s\n", fname);

    //close file
    fclose(fp);

    return arr;
}

//convolution, NOTE: destructive of BLOB* in. duplicate if further required!
BLOB* convolution(BLOB* input, conv_param_t* p){

    //use local pointer
    BLOB* in = input;

    //padding of input if required
    if(p->pad!=0)
        in = pad(in, p->pad);

    //if fully connected, the kernel size is set to the image size
    int Ky=(p->fc)?in->h:p->Ky;
    int Kx=(p->fc)?in->w:p->Kx;

    //create blob to hold output
    int height=(int)floor(((float)in->h - (float)Ky)/(float)p->Sy)+1;
    int width =(int)floor(((float)in->w - (float)Kx)/(float)p->Sx)+1;
    BLOB* out;

    //load bias if required
    if(p->bias==NULL){
        //zero init
        out = blob_calloc(p->num_out, height, width);
    }else{
        //not required to calloc
        out = blob_alloc(p->num_out, height, width);

        //load bias values from file
        float* bias =load_1d(p->bias, p->num_out);

        //set bias or init with zeroes
        for(int o=0;o<out->d;o++)
            for(int m=0;m<out->h;m++)
                for(int n=0;n<out->w;n++)
                    blob_data(out,o,m,n)=bias[o];

        //cleanup bias
        free(bias);
    }

    //load weights
    BLOB* w = load_weights(in, p);

    //perform convolution
    for(int g=0;g<p->group;g++)
        for(int o=g*(out->d/p->group);o<(g+1)*(out->d/p->group);o++)
            for(int i=g*(in->d/p->group);i<(g+1)*(in->d/p->group);i++)
                for(int m=0;m<out->h;m++)
                    for(int n=0;n<out->w;n++)
                        for(int k=0;k<Ky;k++)
                            for(int l=0;l<Kx;l++)
                                //note: absolute starting i is subtracted for the weights, see load_weights function for more info
                                blob_data(out,o,m,n)+=blob_data(in, i, m*p->Sy+k, n*p->Sx+l) * blob_data(w, o, i-(g*(in->d/p->group)), k*Kx + l);

    //free weights
    blob_free(w);

    //done with padded blob, free
    if(p->pad!=0)
        blob_free(in);

    //perform batchnorm if needed
    if(p->bn_mean!=NULL){


        //load batchnorm mean and variance
        float* mean = load_1d(p->bn_mean, out->d);
        float* var  = load_1d(p->bn_var, out->d);

        //batchnorm
        for(int o=0;o<out->d;o++)
            for(int m=0;m<out->h;m++)
                for(int n=0;n<out->w;n++)
                    blob_data(out,o,m,n)= (blob_data(out,o,m,n) - mean[o])/sqrtf(var[o]+p->bn_eps);

        //free mean and variance
        free(mean);
        free(var);
    }

    //perform scale if needed
    if(p->scale!=NULL){
        //load scale parameters
        float* scale = load_1d(p->scale, out->d);
        float* scale_bias = load_1d(p->scale_bias, out->d);

        //scale
        for(int o=0;o<out->d;o++)
            for(int m=0;m<out->h;m++)
                for(int n=0;n<out->w;n++)
                    blob_data(out,o,m,n) = blob_data(out,o,m,n)*scale[o] + scale_bias[o];

        //free parameters
        free(scale);
        free(scale_bias);
    }

    //perform relu
    if(p->relu==true)
        for(int i=0;i<blob_size(out); i++)
            out->data[i] =  fmax(0.0f, out->data[i]);

    //return output
    return out;
}
