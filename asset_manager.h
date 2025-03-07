#include <stdint.h>
#include <string.h>
#include <time.h>

#if !defined(PLASTIC_ASSET_MANAGER)
#define PLASTIC_ASSET_MANAGER

#if defined(__cplusplus)
extern "C" {
#endif

typedef char const *cstr;

typedef enum{
  pla_uncompressed,
  pla_xz,
} pla_asset_compression;

//compressed means xz
typedef struct {
        cstr name;
        time_t last_updated;
        size_t data_size;
        uint8_t const *data;
        pla_asset_compression compression;
} pla_static_asset;

typedef struct {
        cstr name;
        cstr folder;
        uint64_t static_index;
        time_t last_updated;
} pla_asset;

typedef struct {
        cstr asset_folder;
        size_t assets_size;
        pla_asset * assets;
        size_t static_assets_size;
        pla_static_asset const * static_assets;
} pla_asset_manager;


size_t pla_calculate_asset_manager_memory_buffer_size(cstr asset_folder, size_t static_assets_size, pla_static_asset const *static_assets);

//return 0 on fail
int pla_initalize_asset_manager(cstr asset_folder, size_t static_assets_size, pla_static_asset const *static_assets, void * buffer, pla_asset_manager * out_manager);

size_t pla_get_asset_index(cstr name, pla_asset_manager * manager);
//This means that you don't need to alocate a buffer to put the asset in.
int pla_is_uncompressed_static_asset(cstr name, pla_asset_manager * manager);
size_t pla_get_asset_size(cstr name, pla_asset_manager *);

//returns 0 on fail
int pla_load_asset(cstr name, pla_asset_manager * manager, size_t * buffer_size, uint8_t ** buffer);

//returns 1 on change and updated the last_changed time in the manager.
int pla_check_asset_changed(cstr name, pla_asset_manager *);

#define PLASTIC_ASSET_MANAGER_IMPLEMENTATION
#if defined(PLASTIC_ASSET_MANAGER_IMPLEMENTATION)
#include<dirent.h>

size_t allignment = sizeof(void*);

size_t calculate_buffer_with_alignement(size_t size){
        return size + (allignment - (size % allignment));
}

size_t pla_calculate_asset_manager_memory_buffer_size(cstr asset_folder, size_t static_assets_size, pla_static_asset const *static_assets){
        size_t asset_buffer_size = 0;
        size_t string_buffer_size = 0;
        if(asset_folder){
                //TODO:
        }
        if(static_assets && static_assets_size){
                for(size_t i = 0; i < static_assets_size; ++i){
                        //TODO: check for existing file of asset
                        //else
                        asset_buffer_size += 1;
                }
        }
        return calculate_buffer_with_alignement(asset_buffer_size * sizeof(pla_asset)) + string_buffer_size;   
}


// struct c_str_list list_files_in_directory(char_buffer * path){
// 	DIR * dirp = opendir(path->data);
// 	if(dirp == NULL){
// 		printf("unable to open directory %s", path->data);
// 		exit(EXIT_FAILURE);
// 	}
//
// 	struct c_str_list dir_list = initalize_c_str_list();
//
// 	struct dirent * dir = NULL;
// 	do{
// 		dir = readdir(dirp);
// 		if(dir != NULL && dir->d_name[0] != '.'){
// 			push_to_c_str_list(&dir_list, dir->d_name);
// 		}
// 	}while(dir != NULL);
//
// 	return dir_list;
// }

int pla_initalize_asset_manager(cstr asset_folder, size_t static_assets_size, pla_static_asset const *static_assets, void * buffer, pla_asset_manager * out_manager){
        memset(out_manager, 0, sizeof(*out_manager));
        size_t current_asset = 0;
        out_manager->assets = (pla_asset *)buffer;
        if(asset_folder){

        }
        if(static_assets && static_assets_size){
                out_manager->static_assets = static_assets;
                out_manager->static_assets_size = static_assets_size;

                for(size_t i = 0; i < static_assets_size; ++i){
                        pla_asset asset = {
                                .name = out_manager->static_assets[i].name,
                                .folder = NULL,
                                .static_index = i,
                                .last_updated = out_manager->static_assets[i].last_updated,
                        };

                        if(0/*has existing asset in assets folder*/){
                                if(0/*TODO:has newer version in the assets folder.*/) continue;
                                else/*assign current version to index*/ ;
                        }

                        out_manager->assets[current_asset] = asset;
                        ++current_asset;
                }
        }
        out_manager->assets_size = current_asset;
        return 1;
}


size_t pla_get_asset_index(cstr name, pla_asset_manager * manager){
        if(!manager) return SIZE_MAX;
        for(size_t i = 0; i < manager->assets_size; ++i){
                if(strcmp(name, manager->assets[i].name) == 0) return i;
        }
        return SIZE_MAX;
}

int pla_is_uncompressed_static_asset(cstr name, pla_asset_manager * manager){
        if(!manager) return 0;
        size_t index = pla_get_asset_index(name, manager);
        if(index == SIZE_MAX) return 0;
        pla_asset asset = manager->assets[index];
        if(asset.static_index == SIZE_MAX) return 0;
        pla_static_asset static_asset = manager->static_assets[asset.static_index];
        if(asset.last_updated > static_asset.last_updated) return 0;
        if(static_asset.compression == pla_uncompressed) return 1;
        return 0;
}

size_t pla_get_asset_size(cstr name, pla_asset_manager * manager){
        size_t size = 0;
        pla_load_asset(name, manager, &size, NULL);
        return size;
}

size_t dict_space = UINT16_MAX;
uint8_t dict_buffer[UINT16_MAX];

int pla_load_asset(cstr name, pla_asset_manager * manager, size_t * buffer_size, uint8_t ** buffer){
        if(!manager) return 0;
        if(!buffer_size) return 0;
        size_t i = pla_get_asset_index(name, manager);
        if(i == SIZE_MAX) return 0;
        pla_asset asset = manager->assets[i];

        if(asset.static_index == SIZE_MAX) return 0;
        pla_static_asset static_asset = manager->static_assets[asset.static_index];
        //TODO: compair recent versions of assets.

        if(static_asset.compression == pla_uncompressed){
                *buffer_size = static_asset.data_size;
                if(!buffer) return 0;
                //cast away const because it makes the api nicer to deal with.
                *buffer = (uint8_t *)static_asset.data;
        }else{

        }

        if(asset.last_updated > static_asset.last_updated) return 0;
        return 0;
}

int pla_check_asset_changed(cstr name, pla_asset_manager * manager){
        //TODO: chekck asset folder for updated asset.
}

#endif /*PLASTIC_ASSET_MANAGER_IMPLEMENTATION*/

#if defined(__cplusplus)
}
#endif

#endif /*PLASTIC_ASSET_MANAGER*/
