#include <catch.hpp>

#include <algorithm>
#include <fstream>

#include "../AssetFile.h"
#include "../Pack.h"

namespace {
	std::vector<std::string> file_list = { "./constant.fs", "./src/CMakeLists.txt" };
}

TEST_CASE("Save/Load package", "[Pack]") {
	sys::WritePackage("./my_pack.pack", file_list);

	auto OnFile = [](const char *name, void *data, int size){
		auto it = std::find(file_list.begin(), file_list.end(), name);
		REQUIRE(it != file_list.end());
		
		sys::AssetFile in_file(name, sys::AssetFile::IN);
		REQUIRE(in_file.size() == size);

		std::unique_ptr<char[]> buf(new char[size]);
		in_file.Read(buf.get(), size);
		const char *p1 = (char*)data;
		const char *p2 = buf.get();
		REQUIRE(memcmp(p1, p2, size) == 0);
	};
	sys::ReadPackage("./my_pack.pack", OnFile);

	std::vector<sys::FileDesc> list = sys::EnumFilesInPackage("./my_pack.pack");

	REQUIRE(std::string(list[0].name) == "./constant.fs");
	REQUIRE(std::string(list[1].name) == "./src/CMakeLists.txt");
}

TEST_CASE("Add package to AssetFile", "[Pack]") {
    sys::AssetFile::AddPackage("./my_pack.pack");

    sys::AssetFile in_file1("./constant.fs", sys::AssetFile::IN);
    std::ifstream in_file2("./constant.fs", std::ios::ate | std::ios::binary);
    size_t size = (size_t) in_file2.tellg();
    in_file2.seekg(0, std::ios::beg);
    REQUIRE(in_file1.size() == size);

    std::unique_ptr<char[]> buf1(new char[size]), buf2(new char[size]);
    REQUIRE(in_file1.Read(buf1.get(), size));
    in_file2.read(buf2.get(), size);
    REQUIRE(memcmp(buf1.get(), buf2.get(), size) == 0);
}