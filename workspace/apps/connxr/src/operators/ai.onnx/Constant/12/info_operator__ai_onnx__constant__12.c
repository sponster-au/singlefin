//this file was generated by ../../../../../../scripts/onnx_generator/OperatorInfo.py
#include "operators/operator_info.h"
#include "operators/ai.onnx/Constant/12/operator__ai_onnx__constant__12.h"

/* attributes */
static
operator_info_attribute
attributes[] = {
{
    .name     = "sparse_value",
    .optional = false,
    .type     = ONNX__ATTRIBUTE_PROTO__ATTRIBUTE_TYPE__SPARSE_TENSOR
},
{
    .name     = "value",
    .optional = false,
    .type     = ONNX__ATTRIBUTE_PROTO__ATTRIBUTE_TYPE__TENSOR
},
{
    .name     = "value_float",
    .optional = false,
    .type     = ONNX__ATTRIBUTE_PROTO__ATTRIBUTE_TYPE__FLOAT
},
{
    .name     = "value_floats",
    .optional = false,
    .type     = ONNX__ATTRIBUTE_PROTO__ATTRIBUTE_TYPE__FLOATS
},
{
    .name     = "value_int",
    .optional = false,
    .type     = ONNX__ATTRIBUTE_PROTO__ATTRIBUTE_TYPE__INT
},
{
    .name     = "value_ints",
    .optional = false,
    .type     = ONNX__ATTRIBUTE_PROTO__ATTRIBUTE_TYPE__INTS
},
{
    .name     = "value_string",
    .optional = false,
    .type     = ONNX__ATTRIBUTE_PROTO__ATTRIBUTE_TYPE__STRING
},
{
    .name     = "value_strings",
    .optional = false,
    .type     = ONNX__ATTRIBUTE_PROTO__ATTRIBUTE_TYPE__STRINGS
}
};

/* input tensors */
static
operator_info_tensor
inputs[] = {

};

/* output tensors */
static
uint32_t
output_tensor_type_output[] = {
ONNX__TENSOR_PROTO__DATA_TYPE__BOOL,
ONNX__TENSOR_PROTO__DATA_TYPE__COMPLEX128,
ONNX__TENSOR_PROTO__DATA_TYPE__COMPLEX64,
ONNX__TENSOR_PROTO__DATA_TYPE__DOUBLE,
ONNX__TENSOR_PROTO__DATA_TYPE__FLOAT,
ONNX__TENSOR_PROTO__DATA_TYPE__FLOAT16,
ONNX__TENSOR_PROTO__DATA_TYPE__INT16,
ONNX__TENSOR_PROTO__DATA_TYPE__INT32,
ONNX__TENSOR_PROTO__DATA_TYPE__INT64,
ONNX__TENSOR_PROTO__DATA_TYPE__INT8,
ONNX__TENSOR_PROTO__DATA_TYPE__STRING,
ONNX__TENSOR_PROTO__DATA_TYPE__UINT16,
ONNX__TENSOR_PROTO__DATA_TYPE__UINT32,
ONNX__TENSOR_PROTO__DATA_TYPE__UINT64,
ONNX__TENSOR_PROTO__DATA_TYPE__UINT8
};

static
operator_info_tensor
outputs[] = {
{
    .name        = "output",
    .optional    = false,
    .variadic    = false,
    .homogeneous = true,
    .constraint  = "T",
    .n_types     = 15,
    .types       = output_tensor_type_output
}
};

/* constraints */
static
operator_info_constraint
constraints[] = {
{ "T" }
};

/* operator info */
operator_info
info_operator__ai_onnx__constant__12 = {
    .name         = "Constant",
    .range_input  = { 0, 0 },
    .range_output = { 1, 1 },
    .n_attribute  = 8,
    .attribute    = attributes,
    .n_input      = 0,
    .input        = inputs,
    .n_output     = 1,
    .output       = outputs,
    .n_constraint = 1,
    .constraint   = constraints
};