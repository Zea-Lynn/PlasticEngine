#if !defined(PLASTIC_GLTF_H)
#define PLASTIC_GLTF_H

/*
        this is an stb style header  
        PLASTIC_GLTF_IMPLEMENTATION

        Goals:
        Parse gltf into format that can be used by a program to render 3d models.
        Be able to save that format into a file that can be easily loaded into memory, this is so this can be integrated into a pipeline out of blender.
        Foist allocation responsobility onto the caller.
 */

#include <stdint.h>
#include <stdlib.h>


#if defined(__cplusplus)
extern "C" {
#if !defined(PLA_NOEXCEPT)
#define PLA_NOEXCEPT noexcept
#endif
#else
#include <stdbool.h>
#define PLA_NOEXCEPT
#endif

#if !defined(cstr)
#define cstr char const *
#endif

//You can make this static if you are staticaly linking everything.
#if !defined(PLA_EXPORT)
#if defined(PLASTIC_GLTF_IMPLEMENTATION)
#define PLA_EXPORT static
#else
#define PLA_EXPORT extern
#endif /* PLASTIC_GLTF_IMPLEMENTATION */
#endif /* PLA_EXPORT */

typedef size_t pla_offset;

typedef struct {
        size_t size;
        uint8_t const *data;
} pla_str;

typedef struct{
        size_t size;
        pla_offset offset;
} pla_offset_str;

PLA_EXPORT bool pla_str_is_equal(pla_str stra, char const *const strb) PLA_NOEXCEPT; 
PLA_EXPORT bool pla_str_in(pla_str str, char const * const * strs, size_t count) PLA_NOEXCEPT;
PLA_EXPORT bool pla_str_to_int64_t(pla_str str, int64_t * val) PLA_NOEXCEPT;
PLA_EXPORT bool pla_str_to_float(pla_str str, float * val) PLA_NOEXCEPT;

//This is an offset into the buffer, this is to make it easy to serialize the pla version of the GLTF struct.
typedef struct{
        uint32_t size;
        pla_offset offset;
} pla_offset_array;

bool pla_offset_str_is_equal(pla_offset_str stra, char const * const strb) PLA_NOEXCEPT;

typedef struct{
        pla_offset_str generator;
        // Maybe just a number or enum.
        pla_offset_str version;
} pla_GLTF_asset;

typedef struct {
        pla_offset_str name;
        //should point at a pla_GLTF_node type
        pla_offset_array nodes;
} pla_GLTF_scene;

#define pla_GLTF_scene_fields 2;

typedef struct {
        pla_offset_str name;
        uint32_t mesh;
        uint32_t skin;
        //TODO: figure out what to do for this. maybe an arena at the base of gltf object, then this could be an offset. or maybe a union or something.
        union{
                float translation[3];
                float rotation[4];
                float scale[3];
        } transform;
        // uint32_t *children;
        pla_offset_array children;
        uint8_t child_count;
} pla_GLTF_node;

#define PLA_GLTF_MESH_PRIMITIVE_ATTRIBUTES\
        X(POSITION)\
        X(NORMAL)\
        X(TANGENT)\
        /* Require set index. */\
        X(TEXCOORD)\
        X(COLOR)\
        X(JOINTS)\
        X(WEIGHTS)

typedef enum {
        #define X(type) pla_GLTF_##type,
        PLA_GLTF_MESH_PRIMITIVE_ATTRIBUTES
        #undef X
} pla_GLTF_mesh_primitive_attribute_type;

typedef struct {
        uint32_t accessor;
        pla_GLTF_mesh_primitive_attribute_type type;
        int8_t set_index;
} pla_GLTF_mesh_primitive_attribute;

typedef struct {
        // uint8_t attribute_count;
        // pla_GLTF_mesh_primitive_attribute * attributes;
        pla_offset_array attributes;
        uint32_t indices;
        uint32_t material;
        uint32_t mode;
} pla_GLTF_mesh_primitive;

typedef struct {
        pla_offset_str name;
        // pla_GLTF_mesh_primitive *primitives; max 256
        pla_offset_array primitives;
} pla_GLTF_mesh;

#define PLA_GLTF_COMPONENT_TYPE_LIST\
        X(int8_t)\
        X(uint8_t)\
        X(int16_t)\
        X(uint16_t)\
        X(int32_t)\
        X(uint32_t)\
        X(float)

typedef enum {
        pla_GLTF_component_type_none = -1,
        #define X(type) pla_GLTF_component_type_##type,
        PLA_GLTF_COMPONENT_TYPE_LIST
        #undef X
        pla_GLTF_component_type_MAX_ENUM
} pla_GLTF_component_type;

#define PLA_GLTF_TYPE_LIST\
        X(SCALAR)\
        X(VEC2)\
        X(VEC3)\
        X(VEC4)\
        X(MAT2)\
        X(MAT3)\
        X(MAT4)

typedef enum {
        #define X(name)pla_GLTF_##name,
        PLA_GLTF_TYPE_LIST
        #undef X
} pla_GLTF_type;

uint8_t const pla_GLTF_type_component_count[8] = {1,2,3,4,4,9,16};

//min / max values must be cast based off the components array;
#define PLA_ACCESSOR_COMPONENTS \
X(pla_GLTF_component_type,  "componentType",  component_type,  parse_component_type)\
X(pla_GLTF_type,            "type",           type,            parse_gltf_type)\
X(uint32_t,                 "bufferView",     buffer_view,     parse_uint32_t)\
X(uint32_t,                 "byteOffset",     byte_offset,     parse_uint32_t)\
X(pla_offset_array,         "min",            min_values,      parse_min_or_max)\
X(pla_offset_array,         "max",            max_values,      parse_min_or_max)\
X(uint32_t,                 "count",          count,           parse_uint32_t)

typedef struct{
#define X(type, _, prop, __) type prop;
PLA_ACCESSOR_COMPONENTS
#undef X
} pla_GLTF_accessor;

#define BUFFER_VIEW_COMPONENTS \
X(uint32_t,  "buffer",      buffer,       parse_uint32_t)\
X(uint32_t,  "byteLength",  byte_length,  parse_uint32_t)\
X(uint32_t,  "byteOffset",  byte_offset,  parse_uint32_t)\
X(uint32_t,  "byteStride",  byte_stride,  parse_uint32_t)\
X(uint32_t,  "target",      target,       parse_uint32_t)

typedef struct {
#define X(type, _, prop, __) type prop;
BUFFER_VIEW_COMPONENTS
#undef X
} pla_GLTF_buffer_view;

typedef struct {
        pla_str uri;
        size_t byte_length;
} pla_GLTF_buffer;

typedef struct {
        uint32_t sampler;
        struct{
                uint32_t node;
                //TODO: this looks like this can be an enum that relates to the transform type.
                pla_offset_str path;
        } target;
} pla_GLTF_animation_channel;

typedef struct {
        pla_offset_str name;
        //pla_GLTF_channel
        pla_offset_array channels;
        //TODO: make this.
        pla_offset_array samplers;
} pla_GLTF_animation;


#define ROOT_ARRAYS \
        X(scenes, pla_GLTF_scene, "scenes")\
        X(nodes, pla_GLTF_node, "nodes")\
        X(meshes, pla_GLTF_mesh, "meshes")\
        X(accessors, pla_GLTF_accessor, "accessors")\
        X(buffer_views, pla_GLTF_buffer_view, "bufferViews")\
        X(buffers, pla_GLTF_buffer, "buffers")\
        X(animations, pla_GLTF_animation, "animations")\

typedef struct{
        #define X(prop, ...) uint32_t prop;
        ROOT_ARRAYS
        #undef X
        uint32_t scene_nodes;
        uint32_t mesh_primitives;
        uint32_t mesh_primitive_attributes;
        uint32_t animation_channels;
        //when parsing it just dumps the values into the buffer so this needs to be keept track of with offsets in mind.
        uint32_t components[pla_GLTF_component_type_MAX_ENUM];
        uint32_t string_bytes;
        uint32_t indices;
} pla_GLTF_sizes;

typedef struct{
        pla_GLTF_asset asset;
        uint32_t scene;
        #define X(prop, ...) pla_offset_array prop;
        ROOT_ARRAYS
        #undef X
} pla_GLTF;

typedef struct{
        uint32_t size;
        uint32_t type;
        uint8_t const *data;
} pla_GLTF_chunk;

typedef struct{
        uint32_t magic;
        uint32_t version;
        uint32_t length;
        pla_GLTF_chunk json;
        pla_GLTF_chunk binary;
} pla_GLTF_header;

typedef struct{
        //This is convinience and is the same as the buffer pointer.
        pla_GLTF * gltf;
        size_t json_size;
        uint8_t const *json;
        size_t data_size;
        uint8_t const *data;
        //pointer to all dynamic memory needed for the GLTF information.
        void * buffer;
        size_t buffer_size;
        //Internal state for calculating buffer size.
        //TODO: figure out if pre_parse_sizes can just be offsets.
        pla_GLTF_sizes pre_parse_sizes;
        //This turns into the pre_parse_sizes when parse_glb is run.
        pla_GLTF_sizes sizes;
        pla_GLTF_sizes offsets;
} pla_GLTF_state;

PLA_EXPORT bool pla_init_GLTF_state_from_glb(pla_GLTF_state * state, uint8_t const * glb) PLA_NOEXCEPT;

/*Call this after init*/
PLA_EXPORT bool pla_parse_GLTF_glb(pla_GLTF_state * state, void * buffer) PLA_NOEXCEPT;

#if defined(PLASTIC_GLTF_IMPLEMENTATION)
#include <string.h>

typedef struct{
        uint8_t const * data;
        size_t const size;
        size_t offset;
} pla__parse_state;

typedef size_t (*PFN_array_objec_parser)(pla__parse_state, pla_GLTF_state * state, uint32_t index);

PLA_EXPORT bool pla_str_is_equal(pla_str stra, char const *const strb) PLA_NOEXCEPT {
        for(size_t i = 0;i < stra.size;++i) if(stra.data[i] != strb[i]) return false;
        if(strb[stra.size] != '\0') return false;
        return true;
}

PLA_EXPORT bool pla_str_in(pla_str str, char const * const * strs, size_t count) PLA_NOEXCEPT{
        for(size_t i = 0; i < count; ++i) if(pla_str_is_equal(str, strs[i])) return true;
        return false;
}

PLA_EXPORT bool pla_str_to_int64_t(pla_str str, int64_t * val) PLA_NOEXCEPT{
        if(str.size == 0 || str.data == NULL) return 0;
        if(str.size > 20) return 0;
        int64_t value = 0;
        int64_t sign = 1;
        if(str.data[0] == '-') sign = -1;
        for(size_t position = 0 + (sign < 0); position < str.size; ++position){
                size_t digit = str.data[position] - '0';
                bool is_valid = (bool)(digit >= 0 || digit <= 9);
                if(!is_valid) return false;

                value = value * 10 + digit;
        }
        *val = value * sign;
        return true;
}

PLA_EXPORT bool pla_str_to_float(pla_str str, float * val) PLA_NOEXCEPT{
        if(str.size == 0 || str.data == NULL) return 0;
        float sign = 1.0;
        *val = 0;
        if(str.data[0] == '-') sign = -1.0;
        int64_t point_index = -1;
        for(size_t i = 0 + (sign < 0); i < str.size; ++i){
                if(str.data[i] == '.'){
                        point_index = i;
                        continue;
                }
                size_t digit = str.data[i] - '0';
                if(digit >= 0 || digit <= 9){
                        *val = *val * 10.0f + (float)digit;
                        if(point_index >= 0){
                                float decimal = 10.0f;
                                for(size_t b = 1; b < i-point_index; ++b){
                                        decimal *= 10.0f;
                                }
                                *val += (float)digit/decimal;
                        }
                }else return false;
        }
        *val *= sign;
        return true;
}

int8_t pla__check_value_is_mesh_primitive_attribute_name_get_set_index(pla_str value, char const * test_str){
        for(size_t i = 0; i < value.size; ++i){
                if(test_str[i] != value.data[i]){
                        if(value.data[i] == '_'){
                                pla_str set_index_str = {.size = value.size - (i+1), .data = value.data + i + 1, };
                                int64_t index = 0;
                                if(pla_str_to_int64_t(set_index_str, &index)) return index;
                                else return -1;
                        }
                        else return -1;
                } 
        }
        return -1;
}

char const * const pla_GLTF_type_strings[8] = {
#define X(type) #type,
PLA_GLTF_TYPE_LIST
#undef X
};

//returns -1 if the string is not a valid component.
int8_t pla__gltf_lookup_pla_GLTF_component(pla_str str) PLA_NOEXCEPT{
        for(size_t i = 0; i < 8; ++i){
                if(pla_str_is_equal(str, pla_GLTF_type_strings[i])) return i;
        }
        return -1;
}

#define GLTF_MAGIC_glTF 0x46546C67
#define GLTF_MAGIC_JSON 0x4E4F534A
#define GLTF_MAGIC_BIN 0x004E4942

void pla__eat_symbol(pla__parse_state * p, uint8_t symbol){
        if(p->offset == SIZE_MAX) return; 
        if(p->data[p->offset] == symbol) p->offset+=1;
        else p->offset = SIZE_MAX;
}

cstr symbols = "[]{}\",:";

#define PLA__LOOP(iterator, size) for(size_t iterator = 0; iterator < size; ++iterator)

size_t pla__next_symbol(pla__parse_state p){
        if(p.offset == SIZE_MAX) return SIZE_MAX;
        //SIMD?
        for(++p.offset; p.offset < p.size; ++p.offset){
                for(cstr s = symbols; *s != 0; ++s){
                        if(p.data[p.offset] == *s) return p.offset;
                }
        }
        return SIZE_MAX;
}

void pla__parse_string(pla__parse_state * p, pla_str * out_str){
        if(p->offset == SIZE_MAX) return;

        size_t start = p->offset;
        if(p->data[p->offset] != '"'){
                p->offset = SIZE_MAX;
                return;
        } 
        for(++p->offset; p->offset < p->size; ++p->offset){
                if(p->data[p->offset] == '"' && p->data[p->offset-1] != '\\'){
                        if(out_str){
                                out_str->data = p->data+start+1;
                                out_str->size = p->offset - (start+1);
                        }
                        p->offset+=1;
                        return;
                }
        }
        p->offset = SIZE_MAX;
}


//TODO: 
//parses out a value in jason that starts on the first character of the value ends with , or } or ];
void pla__parse_til_close(pla__parse_state * p, pla_str * out_str){ 
        if(p->offset == SIZE_MAX) return;

        size_t start = p->offset;
        for(++p->offset; p->offset < p->size; ++p->offset){
                if(p->data[p->offset] == ',' || p->data[p->offset] == '}' || p->data[p->offset] == ']'){
                        if(out_str){
                                out_str->data = p->data+start;
                                out_str->size = p->offset - (start);
                        }
                        p->offset+=1;
                        return;
                }
        }
        p->offset = SIZE_MAX;
}

//in case theres a key we don't know what to do with
void pla__parse_through(pla__parse_state * p){
        if(p->offset == SIZE_MAX) return;
        uint8_t symbol_stack[UINT8_MAX] = {0};
        int32_t stack_index = -1;
        bool in_string = false;
        for(; p->offset < p->size; ++p->offset) {
                uint8_t symbol = p->data[p->offset];
                if(symbol == '\"'){
                        in_string = true;
                }else if(in_string && symbol == '\"' && p->data[p->offset-1] != '\\'){
                        in_string = false;
                }else if(symbol == '{' || symbol == '['){
                        ++stack_index;
                        symbol_stack[stack_index] = symbol;
                }else if(symbol == '}'){
                        if(stack_index < 0) return;
                        else if(symbol_stack[stack_index] == '{'){
                                --stack_index;
                        }else{
                                //expected symbol was not there malformd json
                                return;
                        }
                } else if(symbol == ']'){
                        if(stack_index < 0) return;
                        else if(symbol_stack[stack_index] == '['){
                                --stack_index;
                        }else{
                                //expected symbol was not there malformd json
                                return;
                        }
                } else if(symbol == ',' && stack_index < 0){
                        return;
                }
        }
        return;
}

void pla__parse_int64_t(pla__parse_state * p, int64_t * out_int){
        if(p->offset == SIZE_MAX) return;
        pla_str value_str = {0};
        pla__parse_til_close(p, &value_str);
        p->offset-=1;
        if(out_int) if(!pla_str_to_int64_t(value_str, out_int)) p->offset = SIZE_MAX;
}
void pla__parse_int64_t_in_array(pla__parse_state * p,  pla_GLTF_state * _, int64_t * out_int){
        (void)_;
        pla__parse_int64_t(p, out_int);
}

void pla__parse_float(pla__parse_state * p, float * out_int){
        if(p->offset == SIZE_MAX) return;
        pla_str value_str = {0};
        pla__parse_til_close(p, &value_str);
        p->offset-=1;
        if(out_int) if(!pla_str_to_float(value_str, out_int)) p->offset = SIZE_MAX;
}
void pla__parse_float_in_array(pla__parse_state * p,  pla_GLTF_state * _, float * out_float){
        (void)_;
        pla__parse_float(p, out_float);
}

void pla__parse_int8_t(pla__parse_state * p, int8_t * out_int){pla__parse_int64_t(p, (int64_t *)out_int);}
void pla__parse_int8_t_in_array(pla__parse_state * p,  pla_GLTF_state * _, int8_t * out_int){ (void)_; pla__parse_int8_t(p, out_int); }
void pla__parse_uint8_t(pla__parse_state * p, uint8_t * out_int){pla__parse_int64_t(p, (int64_t *)out_int);}
void pla__parse_uint8_t_in_array(pla__parse_state * p,  pla_GLTF_state * _, uint8_t * out_int){ (void)_; pla__parse_uint8_t(p, out_int); }
void pla__parse_int16_t(pla__parse_state * p, int16_t * out_int){pla__parse_int64_t(p, (int64_t *)out_int);}
void pla__parse_int16_t_in_array(pla__parse_state * p,  pla_GLTF_state * _, int16_t * out_int){ (void)_; pla__parse_int16_t(p, out_int); }
void pla__parse_uint16_t(pla__parse_state * p, uint16_t * out_int){pla__parse_int64_t(p, (int64_t *)out_int);}
void pla__parse_uint16_t_in_array(pla__parse_state * p,  pla_GLTF_state * _, uint16_t * out_int){ (void)_; pla__parse_uint16_t(p, out_int); }
void pla__parse_int32_t(pla__parse_state * p, int32_t * out_int){pla__parse_int64_t(p, (int64_t *)out_int);}
void pla__parse_int32_t_in_array(pla__parse_state * p,  pla_GLTF_state * _, int32_t * out_int){ (void)_; pla__parse_int32_t(p, out_int); }
void pla__parse_uint32_t(pla__parse_state * p, uint32_t * out_int){pla__parse_int64_t(p, (int64_t *)out_int);}
void pla__parse_uint32_t_in_array(pla__parse_state * p,  pla_GLTF_state * _, uint32_t * out_int){ (void)_; pla__parse_uint32_t(p, out_int); }

#define PLA__PARSE_ARRAY(array, parser, rel_size, state_size) do{\
        pla__eat_symbol(p,  '[');\
        /*TODO: maybe search for an upper bound to this.*/\
        for(uint32_t blargle = 0; blargle < UINT32_MAX; ++blargle){\
                if(p->offset == SIZE_MAX) return;\
                parser(p, state, array ? array + *rel_size : array);\
                if(p->data[p->offset] == ','){\
                        pla__eat_symbol(p,  ',');\
                        ++*rel_size;\
                        ++state->sizes.state_size;\
                }else if(p->data[p->offset] == ']'){\
                        pla__eat_symbol(p,  ']');\
                        ++*rel_size;\
                        ++state->sizes.state_size;\
                        break;\
                }\
        }\
}while(0);

#define pla__parse_string_value(parent, prop)do{\
        pla_str val;\
        pla__parse_string(p, &val);\
        pla_offset_str * offset = NULL;\
        if(parent) offset = &parent->prop;\
        pla__copy_string_to_buffer(state, val, offset);\
}while(0)

#define pla__parse_uint32_value(parent, prop)do{\
        uint32_t * val = NULL;\
        if(parent) val = &parent->prop;\
        pla__parse_uint32_t(p, val);\
}while(0)\

#define PLA__PARSE_KEY\
        if(p->offset == SIZE_MAX) return;\
        pla_str key = {0};\
        pla__parse_string(p, &key);\
        pla__eat_symbol(p,  ':');\

#define PLA__CONTINUE_OR_END_OBJECT\
        if(p->offset == SIZE_MAX) return; \
        if(p->data[p->offset] == ','){\
                p->offset += 1;\
                continue;\
        } \
        else if(p->data[p->offset] == '}'){\
                p->offset+=1;\
                return;\
        }

#define pla__calculate_buffer_ptr(type, prop) state->sizes.prop + (type *)(state->offsets.prop+(uint8_t*)state->buffer)
#define pla__create_offset_array(thing, type) (pla_offset_array){.size = state->pre_parse_sizes.thing, .offset = state->offsets.thing + (state->sizes.thing * sizeof(type)), }
#define pla__from_buffer(offset_array, type) (type *)((uint8_t*)state->buffer + offset_array.offset)

void pla__copy_string_to_buffer(pla_GLTF_state * state, pla_str str, pla_offset_str * out_str){
        if(out_str){
                *out_str = (pla_offset_str){
                        .size = str.size,
                        .offset = state->offsets.string_bytes + state->sizes.string_bytes,
                };
                uint8_t * string = out_str->offset + (uint8_t *)state->buffer;
                memcpy(string, str.data, str.size);
        };
        state->sizes.string_bytes += str.size+1;
}

void pla__parse_scenes(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_scene * scene){
        pla__eat_symbol(p,  '{');
        PLA__LOOP(i, 2){
                PLA__PARSE_KEY
                if(pla_str_is_equal(key, "name")) pla__parse_string_value(scene, name);
                else if(pla_str_is_equal(key, "nodes")){
                        uint32_t * nodes = NULL;
                        uint32_t index;
                        uint32_t * nodes_size = &index;
                        if(state->gltf){
                                pla_GLTF_scene * scene = pla__from_buffer(state->gltf->scenes, pla_GLTF_scene) + state->gltf->scenes.size;
                                scene->nodes = pla__create_offset_array(scene_nodes, *nodes);
                                nodes_size = &scene->nodes.size;
                        }
                        PLA__PARSE_ARRAY(nodes, pla__parse_uint32_t_in_array, nodes_size, scene_nodes)
                }
                else pla__parse_through(p);
                PLA__CONTINUE_OR_END_OBJECT
        }
        p->offset = SIZE_MAX;
}

void pla__parse_nodes(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_node * node){
        pla__eat_symbol(p,  '{');
        PLA__LOOP(i, 5){
                PLA__PARSE_KEY
                if(pla_str_is_equal(key, "name")) pla__parse_string_value(node, name);
                else if(pla_str_is_equal(key, "mesh")) pla__parse_uint32_value(node, mesh);
                else if(pla_str_is_equal(key, "skin")) pla__parse_uint32_value(node, skin);
                else if(pla_str_is_equal(key, "rotation")){ 
                        float * values = NULL;
                        uint32_t index;
                        uint32_t * values_size = &index;
                        if(state->gltf){
                                //TODO:
                        }
                        PLA__PARSE_ARRAY(values, pla__parse_float_in_array, values_size, scene_nodes);
                } else if(pla_str_is_equal(key, "translation")){ 
                        float * values = NULL;
                        uint32_t index;
                        uint32_t * values_size = &index;
                        if(state->gltf){
                                //TODO:
                        }
                        PLA__PARSE_ARRAY(values, pla__parse_float_in_array, values_size, scene_nodes);
                } else if(pla_str_is_equal(key, "scale")){ 
                        float * values = NULL;
                        uint32_t index;
                        uint32_t * values_size = &index;
                        if(state->gltf){
                                //TODO:
                        }
                        PLA__PARSE_ARRAY(values, pla__parse_float_in_array, values_size, scene_nodes);
                } else if(pla_str_is_equal(key, "children")){ 
                        uint32_t * children = NULL;
                        uint32_t index;
                        uint32_t * values_size = &index;
                        if(state->gltf){
                                //TODO:
                        }
                        PLA__PARSE_ARRAY(children, pla__parse_uint32_t_in_array, values_size, scene_nodes);
                }
                else pla__parse_through(p);
                PLA__CONTINUE_OR_END_OBJECT
        }
        p->offset = SIZE_MAX;
}

void pla__parse_mesh_primitive_attributes(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_mesh_primitive * primitive){
        pla__eat_symbol(p, '{');
        pla_GLTF_mesh_primitive_attribute * array = NULL;
        uint32_t count = 0;
        uint32_t * count_ptr = &count;
        if(primitive){
                primitive->attributes.offset = state->offsets.mesh_primitive_attributes + state->sizes.mesh_primitive_attributes * sizeof(pla_GLTF_mesh_primitive_attribute);
                count_ptr = &primitive->attributes.size;
                array = pla__calculate_buffer_ptr(pla_GLTF_mesh_primitive_attribute, mesh_primitive_attributes) + state->sizes.mesh_primitive_attributes;
        }
        PLA__LOOP(i, 69){
                PLA__PARSE_KEY
                pla_str attribute_name = {0};
                int8_t set = 0;
                PLA__LOOP(c, key.size){
                        if(key.data[c] == '_'){
                                attribute_name.data = key.data;
                                attribute_name.size = c;
                                pla_str set_str = {.size = key.size - c, .data = key.data + c};
                                int64_t value = 0;
                                pla_str_to_int64_t(set_str, &value);
                                set = value;
                                break;
                        }
                }
                if(!attribute_name.data) attribute_name = key;
                if(false);
                #define X(type_name) \
                else if(pla_str_is_equal(attribute_name, #type_name)) {\
                        uint32_t * accessor = NULL;\
                        if(primitive) {\
                                (array + *count_ptr)->type = pla_GLTF_##type_name;\
                                (array + *count_ptr)->set_index = set; \
                                accessor = &(array + *count_ptr)->accessor;\
                        }\
                        pla__parse_uint32_t(p, accessor); \
                        *count_ptr+=1;\
                        state->sizes.mesh_primitive_attributes+=1;\
                }
                PLA_GLTF_MESH_PRIMITIVE_ATTRIBUTES
                #undef X
                PLA__CONTINUE_OR_END_OBJECT
        }
        p->offset = SIZE_MAX;
}

void pla__parse_mesh_primitive(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_mesh_primitive * primitive){
        pla__eat_symbol(p,  '{');
        PLA__LOOP(i, 4){
                PLA__PARSE_KEY
                if(pla_str_is_equal(key, "attributes")) pla__parse_mesh_primitive_attributes(p, state, primitive);
                else if(pla_str_is_equal(key, "indices")) pla__parse_uint32_value(primitive, indices);
                else if(pla_str_is_equal(key, "material")) pla__parse_uint32_value(primitive, material);
                else if(pla_str_is_equal(key, "mode")) pla__parse_uint32_value(primitive, mode);
                else pla__parse_through(p);
                PLA__CONTINUE_OR_END_OBJECT
        }
        p->offset = SIZE_MAX;
}

void pla__parse_meshes(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_mesh * mesh){
        pla__eat_symbol(p,  '{');
        if(p->offset == SIZE_MAX) return;
        PLA__LOOP(i, 5){
                PLA__PARSE_KEY
                if(pla_str_is_equal(key, "name")) pla__parse_string_value(mesh, name);
                else if(pla_str_is_equal(key, "primitives")){
                        pla_GLTF_mesh_primitive * primitives = NULL;
                        uint32_t index;
                        uint32_t * primitives_size = &index;
                        if(state->gltf){
                                pla_GLTF_mesh * mesh = pla__from_buffer(state->gltf->meshes, pla_GLTF_mesh) + state->gltf->meshes.size;
                                mesh->primitives = pla__create_offset_array(scene_nodes, *primitives);
                                primitives_size = &mesh->primitives.size;
                        }
                        PLA__PARSE_ARRAY(primitives, pla__parse_mesh_primitive, primitives_size, scene_nodes)
                }
                else pla__parse_through(p);
                PLA__CONTINUE_OR_END_OBJECT
        }
        p->offset = SIZE_MAX;
}

//I know this is not nice, I don't want to break my fingers writing this.
bool pla__parse_accessor_min_max(bool min_or_max, pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_accessor * accessor, pla_GLTF_component_type type){
        switch(type){
                case pla_GLTF_component_type_none: return false;
                #define X(type) case pla_GLTF_component_type_##type:{\
                        type * components_array = NULL;\
                        uint32_t _alal = 0;\
                        uint32_t * components_size_ptr = &_alal;\
                        if(state->gltf){\
                                if(min_or_max){\
                                        accessor->min_values.offset = state->offsets.components[pla_GLTF_component_type_##type] +  state->sizes.components[pla_GLTF_component_type_##type] * sizeof(type);\
                                        components_array = (type *)((uint8_t *)state->buffer + accessor->min_values.offset) + state->sizes.components[pla_GLTF_component_type_##type];\
                                        components_size_ptr = &accessor->min_values.size;\
                                }else{\
                                        accessor->max_values.offset = state->offsets.components[pla_GLTF_component_type_##type] +  state->sizes.components[pla_GLTF_component_type_##type] * sizeof(type);\
                                        components_array = (type *)((uint8_t *)state->buffer + accessor->max_values.offset) + state->sizes.components[pla_GLTF_component_type_##type];\
                                        components_size_ptr = &accessor->max_values.size;\
                                }\
                        }\
                        pla__eat_symbol(p, '[');\
                        for (uint32_t blargle = 0; blargle < (UINT32_MAX); ++blargle) {\
                                if (p->offset == (SIZE_MAX)) return false;\
                                type * component = NULL;\
                                if(components_array) component = components_array+*components_size_ptr;\
                                pla__parse_ ##type## _in_array (p, state, component);\
                                if (p->data[p->offset] == ',') {\
                                        pla__eat_symbol(p, ',');\
                                        ++*components_size_ptr;\
                                        ++state->sizes.components[pla_GLTF_component_type_##type];\
                                } else if (p->data[p->offset] == ']') {\
                                        pla__eat_symbol(p, ']');\
                                        ++*components_size_ptr;\
                                        ++state->sizes.components[pla_GLTF_component_type_##type];\
                                        return true;\
                                }\
                        }\
                return false;}
                PLA_GLTF_COMPONENT_TYPE_LIST
                #undef X
                break;
                default:return false;
        }
}

void pla__parse_accessors(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_accessor * accessor){

        //deffure parsing this until after we know we have the component type.
        pla_GLTF_component_type component_type = pla_GLTF_component_type_none;
        size_t max_offset = SIZE_MAX;
        size_t min_offset = SIZE_MAX;

        pla__eat_symbol(p, '{');
        PLA__LOOP(i, 69){
                PLA__PARSE_KEY
                if(pla_str_is_equal(key, "bufferView")) pla__parse_uint32_value(accessor, buffer_view);
                else if(pla_str_is_equal(key, "componentType")) {
                        uint32_t component_code = 0;
                        pla__parse_uint32_t(p, &component_code);
                        if(p->offset == SIZE_MAX) return;
                        component_type = (pla_GLTF_component_type)(component_code-5120);
                        if(accessor) accessor->component_type = component_type;
                } else if(pla_str_is_equal(key, "count")) pla__parse_uint32_value(accessor, count);
                else if(pla_str_is_equal(key, "max")){
                        max_offset = p->offset;
                        pla__parse_through(p);
                } else if(pla_str_is_equal(key, "min")){
                        min_offset = p->offset;
                        pla__parse_through(p);
                } else if(pla_str_is_equal(key, "type")){
                        pla_str type_str = {0};
                        pla__parse_string(p, &type_str);
                        if(p->offset == SIZE_MAX) return;
                        bool found_str = false;
                        PLA__LOOP(j, sizeof(pla_GLTF_type_strings)/sizeof(*pla_GLTF_type_strings)){
                                if(pla_str_is_equal(type_str, pla_GLTF_type_strings[j])){
                                        found_str = true;
                                        if(accessor) accessor->type = (pla_GLTF_type)j;
                                        break;
                                } 
                        }
                        if(!found_str) p->offset = SIZE_MAX;
                } else pla__parse_through(p); 
                if (p->offset == (SIZE_MAX)) return;
                if (p->data[p->offset] == ',') {
                        p->offset += 1;
                        continue;
                } else if (p->data[p->offset] == '}') {
                        if(component_type == pla_GLTF_component_type_none){
                                p->offset = SIZE_MAX;
                                return;
                        }
                        //TODO as of March 14 2025: this doesn't look like it works when parsing floats of 1 digit.
                        pla__parse_state max_p = {.data = p->data, .size = p->size, .offset = max_offset,};
                        if(max_p.offset != SIZE_MAX){
                                if(!pla__parse_accessor_min_max(0, &max_p, state, accessor, component_type) || p->offset == SIZE_MAX){
                                        p->offset = SIZE_MAX;
                                        return;
                                }
                        }
                        pla__parse_state min_p = {.data = p->data, .size = p->size, .offset = min_offset,};
                        if(max_p.offset != SIZE_MAX){
                                if(!pla__parse_accessor_min_max(1, &min_p, state, accessor, component_type) || p->offset == SIZE_MAX){
                                        p->offset = SIZE_MAX;
                                        return;
                                }
                        }
                        p->offset += 1;
                        return;
                }
        }
        p->offset = SIZE_MAX;
}
void pla__parse_buffer_views(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_buffer_view * buffer_view){
        pla__parse_through(p);
}
void pla__parse_buffers(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_buffer * buffer){
        pla__parse_through(p);
}
void pla__parse_asset(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_asset * asset){
        pla__eat_symbol(p,  '{');
        if(p->offset == SIZE_MAX) return;
        PLA__LOOP(i, 2){
                PLA__PARSE_KEY
                if(pla_str_is_equal(key, "generator")) pla__parse_string_value(asset, generator);
                else if(pla_str_is_equal(key, "version")) pla__parse_string_value(asset, version);
                else pla__parse_through(p);
                PLA__CONTINUE_OR_END_OBJECT
        }
        p->offset = SIZE_MAX;
}

void pla__parse_animation_channel_targets(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_animation_channel * channel){
        pla__eat_symbol(p,  '{');
        PLA__LOOP(j, 2){
                PLA__PARSE_KEY
                if(pla_str_is_equal(key, "path")) pla__parse_string_value(channel, target.path);
                else if(pla_str_is_equal(key, "node")) pla__parse_uint32_value(channel, target.node);
                else pla__parse_through(p);
                PLA__CONTINUE_OR_END_OBJECT
        }
        p->offset = SIZE_MAX;
}
void pla__parse_animation_channels(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_animation_channel * channel){
        pla__eat_symbol(p,  '{');
        if(p->offset == SIZE_MAX) return;
        PLA__LOOP(i, 2){
                PLA__PARSE_KEY
                if(pla_str_is_equal(key, "sampler")) pla__parse_uint32_value(channel, sampler);
                else if(pla_str_is_equal(key, "target")) pla__parse_animation_channel_targets(p, state, channel);
                else pla__parse_through(p);
                PLA__CONTINUE_OR_END_OBJECT
        }
        p->offset = SIZE_MAX;
}

void pla__parse_animations(pla__parse_state * p, pla_GLTF_state * state, pla_GLTF_animation * animation){
        pla__eat_symbol(p,  '{');
        if(p->offset == SIZE_MAX) return;
        PLA__LOOP(i, 4){
                PLA__PARSE_KEY
                if(pla_str_is_equal(key, "channels")) {
                        pla_GLTF_animation_channel * value = NULL;
                        uint32_t index;
                        uint32_t * values_size = &index;
                        if(state->gltf){
                                animation->channels = pla__create_offset_array(animation_channels, *value);
                                values_size = &state->gltf->animations.size;
                        }
                        PLA__PARSE_ARRAY(value, pla__parse_animation_channels, values_size, scene_nodes);
                } 
                // else 
                pla__parse_through(p);
                PLA__CONTINUE_OR_END_OBJECT
        }
        p->offset = SIZE_MAX;
}

void pla__parse_root(pla__parse_state * p, pla_GLTF_state * state){
        if(p->offset == SIZE_MAX) return;
        pla__eat_symbol(p,  '{');
        PLA__LOOP(i, 2 << 4){
                PLA__PARSE_KEY
                if(pla_str_is_equal(key, "asset")){
                pla_GLTF_asset * asset = NULL;
                if(state->gltf) asset = &state->gltf->asset;
                        pla__parse_asset(p, state, asset);
                } else if(pla_str_is_equal(key, "scene")){
                        pla__parse_uint32_value(state->gltf, scene);
                }
                #define X(prop, type, name) else if(pla_str_is_equal(key, name)){\
                        type * prop = NULL;\
                        uint32_t index;\
                        uint32_t * prop##_size = &index;\
                        if(state->gltf){\
                                state->gltf->prop.offset = state->offsets.prop;\
                                prop##_size = &state->gltf->prop.size;\
                                prop = (type *)((uint8_t *)state->buffer + state->offsets.prop);\
                        }\
                        PLA__PARSE_ARRAY(prop, pla__parse_##prop, prop##_size, prop);\
                } 
                ROOT_ARRAYS
                #undef X
                else pla__parse_through(p);
                PLA__CONTINUE_OR_END_OBJECT
        }
        p->offset = SIZE_MAX;
}

uint32_t pla__calculate_aligned_offset(uint32_t offset){
        static size_t alignment = sizeof(void *);
        return offset +(alignment - (offset % alignment));
}

bool pla__calculate_buffer_offsets(pla_GLTF_state * state) PLA_NOEXCEPT{
        if(!state) return false;
        uint32_t current_offset = 0;
        current_offset += pla__calculate_aligned_offset(sizeof(*state->gltf));
        #define calculate_new_offset(prop, size) state->offsets.prop = current_offset; current_offset += pla__calculate_aligned_offset(size)
        #define calculate_new_offset_with_type(prop, type) state->offsets.prop = calculate_new_offset(prop, sizeof(type) * state->sizes.prop)
        #define X(prop, type, _) calculate_new_offset(prop, sizeof(type) * state->sizes.prop);
        ROOT_ARRAYS
        #undef X
        calculate_new_offset_with_type(scene_nodes, pla_GLTF_node);
        calculate_new_offset_with_type(mesh_primitives, pla_GLTF_mesh_primitive);
        calculate_new_offset_with_type(mesh_primitive_attributes, pla_GLTF_mesh_primitive_attribute);
        #define X(type) calculate_new_offset_with_type(components[pla_GLTF_component_type_##type], type);
        PLA_GLTF_COMPONENT_TYPE_LIST
        #undef X
        state->offsets.string_bytes = calculate_new_offset(string_bytes, state->sizes.string_bytes); 
        state->buffer_size = current_offset;
        return true;
}

PLA_EXPORT bool pla_init_GLTF_state_from_glb(pla_GLTF_state * state, uint8_t const * glb) PLA_NOEXCEPT{
        if(!state) return false;
        if(!glb) return false;
        memset(state, 0, sizeof(pla_GLTF_state));

        pla_GLTF_header header = {0};
        memcpy(&header.magic, glb, 4);
        if (header.magic != GLTF_MAGIC_glTF) return false;
        memcpy(&header.version, glb + 4, 4);
        memcpy(&header.length, glb + 8, 4);
        memcpy(&header.json.size, glb + 12, 4);
        memcpy(&header.json.type, glb + 16, 4);
        if (header.json.type != GLTF_MAGIC_JSON) return false;
        header.json.data = glb + 20;
        memcpy(&header.binary.size, glb + 20 + header.json.size, 4);
        memcpy(&header.binary.type, glb + 20 + header.json.size + 4, 4);
        if (header.binary.type != GLTF_MAGIC_BIN) return false;
        header.binary.data = glb + 20 + header.json.size + 8;

        state->data = header.binary.data;
        state->data_size = header.binary.size;
        state->json = header.json.data;
        state->json_size = header.json.size;
        pla__parse_state p = {.data = state->json, .size = state->json_size, .offset = 0};
        pla__parse_root(&p, state);
        if(p.offset == SIZE_MAX) return false;
        if(!pla__calculate_buffer_offsets(state)) return false;
        return true;
}

PLA_EXPORT bool pla_parse_GLTF_glb(pla_GLTF_state * state, void * buffer) PLA_NOEXCEPT{
        if(!state) return false;
        if(!buffer) return false;
        state->pre_parse_sizes = state->sizes;
        state->sizes = (pla_GLTF_sizes){0};
        state->buffer = buffer;
        state->gltf = (pla_GLTF *)buffer;
        memset(buffer, 0, sizeof(*state->gltf));
        pla__parse_state p = {.data = state->json, .size = state->json_size, .offset = 0};
        pla__parse_root(&p, state);
        return p.offset != SIZE_MAX;
}

#endif /* PLASTIC_GLTF_IMPLEMENTATION */
#if defined(__cplusplus)
}
#endif
#endif /* PLASTIC_GLTF_H */
