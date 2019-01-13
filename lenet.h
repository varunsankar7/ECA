#ifndef LE_NET_V2
#define LE_NET_V2
#include "network.h"

//list of network layers
layer_t lenet_layers[]={

    //data (idx=0)
        //first layer is the input image
    //end data

    { //conv1 (idx=1)
        .name="conv1",
        .type=CONVOLUTION,
        .input=0,
        .input2=-1,
        .param = { .conv = {
            //number of output feature maps
            .num_out=20,

            //kernel sizes
            .Ky=5,
            .Kx=5,

            //strides
            .Sy=1,
            .Sx=1,

            //padding
            .pad=0,

            //group
            .group=1,

            //weights
            .weights="bins/conv1_weights.bin",

            //bias
            .bias="bins/conv1_bias.bin",

            //scaling
            .scale=NULL,
            .scale_bias=NULL,

            //batch norm
            .bn_mean=NULL,
            .bn_var=NULL,
            .bn_eps=0,

            //relu
            .relu=false,

            //fully connected
            .fc=false
       }}
    },//end of conv1

    { //pool1 (idx=2)
        .name="pool1",
        .type=POOLING,
        .input=1,
        .input2=-1,
        .param={ .pool={
            .type=POOL_MAX,
            .global=false,
            .Kx=2,
            .Ky=2,
            .Sx=2,
            .Sy=2,
        }}
    },//end of pool1

    { //conv2 (idx=3)
        .name="conv1",
        .type=CONVOLUTION,
        .input=2,
        .input2=-1,
        .param = { .conv = {
            //number of output feature maps
            .num_out=50,

            //kernel sizes
            .Ky=5,
            .Kx=5,

            //strides
            .Sy=1,
            .Sx=1,

            //padding
            .pad=0,

            //group
            .group=1,

            //weights
            .weights="bins/conv2_weights.bin",

            //bias
            .bias="bins/conv2_bias.bin",

            //scaling
            .scale=NULL,
            .scale_bias=NULL,

            //batch norm
            .bn_mean=NULL,
            .bn_var=NULL,
            .bn_eps=0,

            //relu
            .relu=false,

            //fully connected
            .fc=false
       }}
    },//end of conv2


    { //pool2 (idx=4)
        .name="pool2",
        .type=POOLING,
        .input=3,
        .input2=-1,
        .param={ .pool={
            .type=POOL_MAX,
            .global=false,
            .Kx=2,
            .Ky=2,
            .Sx=2,
            .Sy=2,
        }}
    },//end of pool2

    { //ip1 (idx=5)
        .name="ip1",
        .type=CONVOLUTION,
        .input=4,
        .input2=-1,
        .param = { .conv = {
            //number of output feature maps
            .num_out=500,

            //kernel sizes
            .Ky=1,
            .Kx=1,

            //strides
            .Sy=1,
            .Sx=1,

            //padding
            .pad=0,

            //group
            .group=1,

            //weights
            .weights="bins/ip1_weights.bin",

            //bias
            .bias="bins/ip1_bias.bin",

            //scaling
            .scale=NULL,
            .scale_bias=NULL,

            //batch norm
            .bn_mean=NULL,
            .bn_var=NULL,
            .bn_eps=0,

            //relu
            .relu=true,

            //fully connected
            .fc=true
        }}
    },//end of ip1

    { //ip2 (idx=6)
        .name="ip2",
        .type=CONVOLUTION,
        .input=5,
        .input2=-1,
        .param = { .conv = {
            //number of output feature maps
            .num_out=10,

            //kernel sizes
            .Ky=1,
            .Kx=1,

            //strides
            .Sy=1,
            .Sx=1,

            //padding
            .pad=0,

            //group
            .group=1,

            //weights
            .weights="bins/ip2_weights.bin",

            //bias
            .bias="bins/ip2_bias.bin",

            //scaling
            .scale=NULL,
            .scale_bias=NULL,

            //batch norm
            .bn_mean=NULL,
            .bn_var=NULL,
            .bn_eps=0,

            //relu
            .relu=false,

            //fully connected
            .fc=true
        }}
    },//end of ip2

    {
        //last layer indicator
        .name=NULL,
        .type=NONE,
        .input=-1,
        .input2=-1,
        .param = { .eltwise = { .type = EW_SUM  }}
    }
}; //end of layers

Network lenet={
    .name="LeNet",
    .layers=lenet_layers
};


const char* labels[]={
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9"
};
#endif
