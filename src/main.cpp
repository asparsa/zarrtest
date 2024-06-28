#include <tensorstore/tensorstore.h>
#include <tensorstore/util/status.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tensorstore/context.h"
#include "tensorstore/open.h"
#include "tensorstore/open_mode.h"
#include "tensorstore/index.h"
#include <tensorstore/index_space/dim_expression.h>
#include <typeinfo>
namespace {

namespace kvstore = tensorstore::kvstore;
using ::tensorstore::ChunkLayout;
using ::tensorstore::Context;
using ::tensorstore::DimensionIndex;
using ::tensorstore::DimensionSet;}

::nlohmann::json GetJsonSpec() {
  return {
{"driver", "zarr3"},
      {"kvstore", {{"driver", "file"}, {"path", "testo"}}},
      {"metadata",
       {
           {"data_type", "int16"},
           {"shape", {100,100}},
           {"chunk_grid", {
                {"name", "regular"},
                {"configuration", {
                    {"chunk_shape", {10,10}}
                }}
       }}
  }}
};
  }

//just a function to create the zarr file	  
static auto create_zarr(){
	::nlohmann::json json_spec = GetJsonSpec();
	auto context = Context::Default();
	auto store_result = tensorstore::Open(json_spec, context, tensorstore::OpenMode::create |tensorstore::OpenMode::open,tensorstore::ReadWriteMode::read_write).result(); 
	return store_result;}

//try to create a zarr and write a 100*100 random array in it	
int main() {
	      ::nlohmann::json json_spec = GetJsonSpec();
	auto store_result=create_zarr();	
	if (!store_result.ok()) {
			      std::cerr << "Error creating Zarr file: " << std::endl;
			          return -1;
				    }

		    std::cout << "Zarr file created successfully!" << std::endl;
srand(time(0));
int randomNumber;
auto array = tensorstore::AllocateArray<int16_t>({100,100});
for (tensorstore::Index i = 0; i < 100; ++i) {
        for (tensorstore::Index j = 0; j < 100; ++j) {
            randomNumber = (rand() % 10) + 1;
		array(i, j) = randomNumber;
        }
}
	auto write_result= tensorstore::Write( array,store_result).status();
	if (!write_result.ok()) {
                              std::cerr << "Error creating Zarr file: " << std::endl;
                                  return -1;
                                    }

std::cout << "Zarr file wrote successfully!" << std::endl;
auto read=tensorstore::Open( GetJsonSpec(),tensorstore::OpenMode::open, tensorstore::ReadWriteMode::read_write).result();
if (!read.ok()) {
        std::cerr << "Error opening TensorStore: " << read.status() << std::endl;
        return 1;
    }
return 0;
}
