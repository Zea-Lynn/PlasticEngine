#if !defined(PLASTIC_GLTF_H)
#define PLASTIC_GLTF_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//this is an stb style header  
//PLASTIC_GLTF_IMPLEMENTATION

#if defined(__cplusplus)
extern "C" {
#if !defined(PLA_PLA_NOEXCEPT)
#define PLA_PLA_NOEXCEPT noexcept
#endif
#else
#define PLA_NOEXCEPT
#endif

#if !defined(cstr)
typedef char const * cstr;
#endif


typedef struct pla_str {
        uint8_t const *data;
        size_t size;
} pla_str;
bool pla_str_is_equal(pla_str stra, char const *const strb) PLA_NOEXCEPT; 
bool pla_str_in(pla_str str, char const * const * strs, size_t count) PLA_NOEXCEPT;
ssize_t pla_str_to_ssize_t(pla_str str) PLA_NOEXCEPT;
float pla_str_to_float(pla_str str) PLA_NOEXCEPT;

typedef struct{
        pla_str generator;
        // Maybe just a number or enum.
        pla_str version;
} pla_GLTF_asset;

typedef struct {
        pla_str name;
        uint32_t *nodes;
        uint8_t node_count;
} pla_GLTF_scene;

typedef struct {
        pla_str name;
        uint32_t mesh;
        uint32_t skin;
        //TODO: figure out what to do for this. maybe an arena at the base of gltf object, then this could be an offset. or maybe a union or something.
        // float translation[3];
        // float rotation[4];
        // float scale[3];
        uint32_t *children;
        uint8_t child_count;
} pla_GLTF_node;

typedef enum {
        pla_POSITION,
        pla_NORMAL,
        pla_TANGENT,

        //Require set index.
        pla_TEXCOORD,
        pla_COLOR,
        pla_JOINTS,
        pla_WEIGHTS,
} pla_GLTF_mesh_primitive_attribute_name;

typedef struct {
        uint32_t accessor;
        pla_GLTF_mesh_primitive_attribute_name name;
        int8_t set_index;
}pla_GLTF_mesh_primitive_attribute;


typedef struct {
        uint8_t attribute_count;
        pla_GLTF_mesh_primitive_attribute * attributes;
        uint32_t indices;
        uint32_t material;
        uint32_t mode;
}pla_GLTF_mesh_primitive;

typedef struct {
        pla_str name;
        pla_GLTF_mesh_primitive *primitives;
        uint8_t primitive_count;
} pla_GLTF_mesh;

typedef enum {
        pla_GLTF_component_type_none = -1,
        pla_GLTF_component_type_int8_t,
        pla_GLTF_component_type_uint8_t,
        pla_GLTF_component_type_int16_t,
        pla_GLTF_component_type_uint16_t,
        pla_GLTF_component_type_uint32_t,
        pla_GLTF_component_type_float,
} pla_GLTF_component_type;


uint8_t const pla_GLTF_component_type_byte_count[6] = {1,1,2,2,4,4};

typedef enum {
        pla_GLTF_SCALAR,
        pla_GLTF_VEC2,
        pla_GLTF_VEC3,
        pla_GLTF_VEC4,
        pla_GLTF_MAT2,
        pla_GLTF_MAT3,
        pla_GLTF_MAT4,
} pla_GLTF_type;

uint8_t const pla_GLTF_type_component_count[8] = {1,2,3,4,4,9,16};

//min / max values must be cast based off the components array;
#define ACESSOR_COMPONENTS \
        X(pla_GLTF_component_type, "componentType", component_type, parse_component_type)\
        X(pla_GLTF_type, "type", type, parse_gltf_type)\
        X(uint32_t, "bufferView", buffer_view, parse_uint32_t)\
        X(uint32_t, "byteOffset", byte_offset, parse_uint32_t)\
        X(void *, "min", min_values, parse_min_or_max)\
        X(void *, "max", max_values, parse_min_or_max)\
        X(uint32_t, "count", count, parse_uint32_t)

typedef struct{
#define X(type, _, prop, __) type prop;
ACESSOR_COMPONENTS
#undef X
} pla_GLTF_accessor;

#define BUFFER_VIEW_COMPONENTS \
        X(uint32_t, "buffer", buffer, parse_uint32_t)\
        X(uint32_t, "byteLength", byte_length, parse_uint32_t)\
        X(uint32_t, "byteOffset", byte_offset, parse_uint32_t)\
        X(uint32_t, "byteStride", byte_stride, parse_uint32_t)\
        X(uint32_t, "target", target, parse_uint32_t)

typedef struct {
#define X(type, _, prop, __) type prop;
BUFFER_VIEW_COMPONENTS
#undef X
} pla_GLTF_buffer_view;

typedef struct {
        pla_str uri;
        size_t byte_length;
} pla_GLTF_buffer;

typedef struct pla_allocator {
        void * user_data;
        void *(*allocate)(void * user_data, size_t size);
        void (*free)(void * user_data, void *ptr);
} pla_allocator;

#define ROOT_ARRAYS \
        X(pla_GLTF_scene *, "scenes", scenes)\
        X(pla_GLTF_node *, "nodes", nodes)\
        X(pla_GLTF_mesh *, "meshes", meshes)\
        X(pla_GLTF_accessor *, "accessors", accessors)\
        X(pla_GLTF_buffer_view *, "bufferViews", buffer_views)\
        X(pla_GLTF_buffer *, "buffers", buffers)

typedef struct{
        #define X(type, _, prop) uint32_t prop;
        ROOT_ARRAYS
        #undef X
        uint32_t mesh_primitives;
        uint32_t mesh_primitive_attributes;
} pla_GLTF_sizes;

typedef struct{
        #define X(type, _, prop) type prop;
        ROOT_ARRAYS
        #undef X
        pla_GLTF_mesh_primitive * mesh_primitives;
        pla_GLTF_mesh_primitive_attribute * mesh_primitive_attributes;
}pla_GLTF_arena;

//this needs to be zero initalized.
typedef struct {
        uint32_t version;
        pla_GLTF_asset asset;
        uint32_t scene;
        #define X(type, name, prop) type prop;
        ROOT_ARRAYS
        #undef X
        uint32_t accessor_count;
        uint32_t scene_count;
        uint32_t node_count;
        uint32_t mesh_count;
        uint32_t buffer_view_count;
        uint32_t buffer_count;

        //Non Owning this points into the raw_gltf_data passed in by the caller.
        uint8_t const *data;
        size_t data_size;

        //These structs can be set by the caller the reason that you would want to do that each arena can be part of a larger set of arenas that is used by more models.
        pla_GLTF_sizes sizes;
        pla_GLTF_arena * arena;
} pla_GLTF;

typedef struct {
        uint32_t magic;
        uint32_t version;
        uint32_t length;
} pla_GLTF_header;

typedef struct {
        uint32_t size;
        uint32_t type;
        uint8_t const *data;
} pla_GLTF_chunk;

//returns false on failure.
//You are supposed to use this by calling it twice, once for the size of the buffer and again to fill out the data.
bool pla_parse_GLTF(uint8_t const * glb, pla_GLTF * gltf, size_t * buffer_size, uint8_t * buffer);

#if defined(PLASTIC_GLTF_IMPLEMENTATION)

typedef struct{
        uint8_t const * data;
        size_t const size;
        size_t offset;
} pla__parse_state;


//Internal state.
typedef struct{
        pla_GLTF * state;
        pla_GLTF_sizes current_sizes;
} pla__GLTF;

typedef size_t (*PFN_array_objec_parser)(pla__parse_state, pla__GLTF * gltf, uint32_t index);

bool pla_str_is_equal(pla_str stra, char const *const strb) PLA_NOEXCEPT {
        for(size_t i = 0;i < stra.size;++i) if(stra.data[i] != strb[i]) return false;
        if(strb[stra.size] != '\0') return false;
        return true;
}

bool pla_str_in(pla_str str, char const * const * strs, size_t count) PLA_NOEXCEPT{
        for(size_t i = 0; i < count; ++i) if(pla_str_is_equal(str, strs[i])) return true;
        return false;
}

ssize_t pla_str_to_ssize_t(pla_str str) PLA_NOEXCEPT{
        if(str.size == 0 || str.data == NULL) return 0;
        if(str.size > 20) return 0;
        ssize_t value = 0;
        ssize_t sign = 1;
        if(str.data[0] == '-') sign = -1;
        for(size_t position = 0 + (sign < 0); position < str.size; ++position){
                size_t digit = str.data[position] - '0';
                bool is_valid = (bool)(digit >= 0 || digit <= 9);
                value = value * 10 + (digit * is_valid);
        }

        return value * sign;
}

float pla_str_to_float(pla_str str) PLA_NOEXCEPT{
        if(str.size == 0 || str.data == NULL) return 0;
        float sign = 1;
        float value = 0;
        if(str.data[0] == '-') sign = -1;
        ssize_t point_index = -1;
        for(size_t i = 0 + (sign < 0); i < str.size; ++i){
                if(str.data[i] == '.'){
                        point_index = i;
                        continue;
                }
                size_t digit = str.data[i] - '0';
                if(digit >= 0 || digit <= 9){
                        value = value * 10.0f + (float)digit;
                        if(point_index >= 0){
                                float decimal = 10;
                                for(size_t b = 1; b < i-point_index; ++b){
                                        decimal *= 10.0f;
                                }
                                value += (float)digit/decimal;
                        }
                }
        }
        return value * sign;
}

int8_t pla__check_value_is_mesh_primitive_attribute_name_get_set_index(pla_str value, char const * test_str){
        for(size_t i = 0; i < value.size; ++i){
                if(test_str[i] != value.data[i]){
                        if(value.data[i] == '_'){
                                pla_str set_index_str = {.data = value.data + i + 1, .size = value.size - (i+1)};
                                return pla_str_to_ssize_t(set_index_str);
                        }
                        else return -1;
                } 
        }
        return -1;
}

char const * const pla_GLTF_type_strings[8] = { "SCALAR", "VEC2", "VEC3", "VEC4", "MAT2", "MAT3", "MAT4" };

//returns -1 if the string is not a valid component.
int8_t pla__gltf_lookup_pla_GLTF_component(pla_str str) PLA_NOEXCEPT{
        for(size_t i = 0; i < 8; ++i){
                if(pla_str_is_equal(str, pla_GLTF_type_strings[i])) return i;
        }
        return -1;
}

char const * const pla_GLTF_component_type_strings[6] = {"5120","5121","5122","5123","5125","5126"};



//returns -1 if the string is not a valid type code.
pla_GLTF_component_type pla__gltf_lookup_component_type(pla_str value) PLA_NOEXCEPT{
        for(size_t i = 0; i < 6; ++i){
                if(pla_str_is_equal(value, pla_GLTF_component_type_strings[i])) return (pla_GLTF_component_type)i;
        }
        return pla_GLTF_component_type_none;
}

#define GLTF_MAGIC_glTF 0x46546C67
#define GLTF_MAGIC_JSON 0x4E4F534A
#define GLTF_MAGIC_BIN 0x004E4942

size_t pla__eat_symbols(pla__parse_state p, cstr symbols){
        if(p.offset == SIZE_MAX) return SIZE_MAX;
        for(;*symbols != 0; ++symbols) if(p.data[p.offset] == *symbols) return p.offset+1;
        return SIZE_MAX;
}

cstr symbols = "[]{}\",:";

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

size_t pla__parse_string(pla__parse_state p, pla_str * out_str){
        if(p.offset == SIZE_MAX) return SIZE_MAX;
        size_t start = p.offset;
        if(p.data[p.offset] != '"') return SIZE_MAX;
        for(++p.offset; p.offset < p.size; ++p.offset){
                if(p.data[p.offset] == '"' && p.data[p.offset-1] != '\\'){
                        if(out_str){
                                out_str->data = p.data+start+1;
                                out_str->size = p.offset - (start+1);
                        }
                        return p.offset;
                }
        }
        return SIZE_MAX;
}

size_t pla__parse_array(pla__parse_state p, pla__GLTF * gltf, PFN_array_objec_parser parser){
        uint32_t index = 0;
        if(p.offset == SIZE_MAX) return SIZE_MAX;
        p.offset = pla__eat_symbols(p, "[");
        while(1){
                p.offset = parser(p, gltf, index);
                if(p.data[p.offset] == ','){
                        p.offset = pla__eat_symbols(p, ",");
                        ++index;
                }else if(p.data[p.offset] == ']'){
                        p.offset = pla__eat_symbols(p, "]");
                        return p.offset;
                }
        }
        return SIZE_MAX;
}

size_t pla__parse_scene_nodes(pla__parse_state, pla__GLTF * gltf, uint32_t index){

}
size_t pla__parse_scenes(pla__parse_state p, pla__GLTF *gltf, uint32_t index){
        if(p.offset == SIZE_MAX) return SIZE_MAX;
        pla_str * name = NULL;

        if(gltf->state->arena){
                if(gltf->current_sizes.scenes+1 > gltf->state->sizes.scenes) return SIZE_MAX;
                if(index == 0){
                        gltf->state->scenes = gltf->state->arena->scenes + gltf->current_sizes.scenes;
                }
                name = &gltf->state->scenes[index].name;
                ++gltf->state->scene_count;
        } 

        ++gltf->current_sizes.scenes;

        while(1){
                if(p.offset == SIZE_MAX) return SIZE_MAX;
                pla_str key = {0};
                p.offset = pla__parse_string(p, &key);
                p.offset = pla__eat_symbols(p, ":");
                if(pla_str_is_equal(key, "name")){
                        p.offset = pla__parse_string(p, name);
                } else if(pla_str_is_equal(key, "nodes")){
                        p.offset = pla__parse_array(p, gltf, pla__parse_scene_nodes);
                }

                if(p.offset == SIZE_MAX) return SIZE_MAX;
                if(p.data[p.offset] == ',') continue;
                else if(p.data[p.offset] == '}') return p.offset;
        }

        return p.offset;
}

size_t pla__parse_nodes(pla__parse_state p, pla__GLTF *gltf, uint32_t index){
        if(p.offset == SIZE_MAX) return SIZE_MAX;
}
size_t pla__parse_meshes(pla__parse_state p, pla__GLTF *gltf, uint32_t index){
        if(p.offset == SIZE_MAX) return SIZE_MAX;
}
size_t pla__parse_accessors(pla__parse_state p, pla__GLTF *gltf, uint32_t index){
        if(p.offset == SIZE_MAX) return SIZE_MAX;
}
size_t pla__parse_buffer_views(pla__parse_state p, pla__GLTF *gltf, uint32_t index){
        if(p.offset == SIZE_MAX) return SIZE_MAX;
}
size_t pla__parse_buffers(pla__parse_state p, pla__GLTF *gltf, uint32_t index){
        if(p.offset == SIZE_MAX) return SIZE_MAX;
}

bool pla_parse_GLTF(uint8_t const * glb, pla_GLTF * gltf, size_t * buffer_size, uint8_t * buffer){
        if(!gltf) return false;

        pla_GLTF_header header;
        pla_GLTF_chunk json_chunk;
        pla_GLTF_chunk binary_chunk;

        if(!gltf->data){
                memcpy(&header.magic, glb, 4);
                if (header.magic != GLTF_MAGIC_glTF) return false;
                memcpy(&header.version, glb + 4, 4);
                memcpy(&header.length, glb + 8, 4);
                memcpy(&json_chunk.size, glb + 12, 4);
                memcpy(&json_chunk.type, glb + 16, 4);
                if (json_chunk.type != GLTF_MAGIC_JSON) return false;
                json_chunk.data = glb + 20;
                memcpy(&binary_chunk.size, glb + 20 + json_chunk.size, 4);
                memcpy(&binary_chunk.type, glb + 20 + json_chunk.size + 4, 4);
                if (binary_chunk.type != GLTF_MAGIC_BIN) return false;
                binary_chunk.data = glb + 20 + json_chunk.size + 8;
                gltf->data = binary_chunk.data;
                gltf->data_size = binary_chunk.size;
        }

        pla__parse_state p = {.data = json_chunk.data, .size = json_chunk.size};
        pla__GLTF state = {.state = gltf, .current_sizes = {0}};

        p.offset = pla__eat_symbols(p, "{");
        while(1){
                if(p.offset == SIZE_MAX) return false;
                pla_str key = {0};
                p.offset = pla__parse_string(p, &key);
                p.offset = pla__eat_symbols(p, ":");

                if(pla_str_is_equal(key, "asset")){

                } else if(pla_str_is_equal(key, "scene")){

                }
                #define X(type, _, prop) pla__parse_array(p, gltf, pla__parse_##prop(p, &state));
                ROOT_ARRAYS
                #undef X

                if(p.data[p.offset] == ',') continue;
                else if(p.data[p.offset] == '}') return true;
        }
        return false;
}

#endif /* PLASTIC_GLTF_IMPLEMENTATION */
#if defined(__cplusplus)
}
#endif
#endif /* PLASTIC_GLTF_H */
