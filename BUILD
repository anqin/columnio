# generated by genlibbuild

cc_library(
    name = 'io',
    srcs = [],
    deps = [
        ':tablet_reader',
        ':slice_writer',
    ]
)

proto_library(
    name = 'column_metadata_proto',
    srcs = 'column_metadata.proto',
    deps = []
)

proto_library(
    name = 'tablet_schema_proto',
    srcs = 'tablet_schema.proto',
    deps = [
        ':column_metadata_proto',
    ]
)

proto_library(
    name = 'table_options_proto',
    srcs = 'table_options.proto',
    deps = [
        ':tablet_schema_proto',
    ]
)

cc_library(
    name = 'slice_writer',
    srcs = 'slice_writer.cc',
    deps = [
        ':slice',
        ':tablet_writer',
        '//twister/common/base:twister_flags',
    ]
)

cc_test(
    name = 'slice_writer_mock_test',
    srcs = ['slice_writer_mock_test.cc'],
    deps = [
        ':slice_writer',
        '//thirdparty/gmock:gmock',
    ]
)

cc_test(
    name = 'slice_writer_test',
    srcs = ['slice_writer_test.cc'],
    deps = [
        ':slice_writer',
        ':tablet_writer',
        '//twister/utils:proto_message',
    ],
    testdata = [
        'testdata/document.proto',
    ]
)

cc_library(
    name = 'tablet_writer',
    srcs = [
        'column_writer.cc',
        'tablet_writer.cc',
    ],
    deps = [
        ':block',
        ':tablet_schema_proto',
        '//twister/common/base:data_holder',
        '//twister/utils:proto_message',
        '//common/file:file',
        '//common/file:local_file',
        '//common/system/uuid:uuid',
    ]
)

cc_test(
    name = 'column_writer_test',
    srcs = ['column_writer_test.cc'],
    deps = [
        ':tablet_writer',
    ]
)

cc_test(
    name = 'tablet_writer_test',
    srcs = ['tablet_writer_test.cc'],
    deps = [
        ':tablet_writer',
    ]
)

cc_library(
    name = 'tablet_reader',
    srcs = [
        'column_reader.cc',
        'tablet_reader.cc',
    ],
    deps = [
        ':slice',
        ':tablet_schema_proto',
        '//common/file:file',
        '//common/file:local_file',
        '//common/system/concurrency:concurrency',
        '//common/system/memory:memory',
        '//twister/utils:proto_message',
    ]
)

cc_test(
    name = 'tablet_reader_test',
    srcs = ['tablet_reader_test.cc'],
    deps = [
        ':column_metadata_proto',
        ':io_test_helper',
        ':table_builder',
        ':table_options_proto',
        ':tablet_reader',
        '//common/file/recordio:recordio',
        '//twister/io/testdata:document_proto',
    ],
    testdata = [
        'testdata/document.proto',
    ]
)

cc_test(
    name = 'column_reader_test',
    srcs = ['column_reader_test.cc'],
    deps = [
        ':tablet_reader',
    ]
)

cc_library(
    name = 'table_builder',
    srcs = 'table_builder.cc',
    deps = [
        ':block',
        ':column_metadata_proto',
        ':field_productor',
        ':table_options_proto',
        ':tablet_schema_proto',
        ':tablet_writer',
    ]
)

cc_test(
    name = 'table_builder_test',
    srcs = 'table_builder_test.cc',
    deps = [
        ':table_builder',
    ]
)

cc_test(
    name = 'table_builder_mock_test',
    srcs = 'table_builder_mock_test.cc',
    deps = [
        ':table_builder',
        '//thirdparty/gmock:gmock',
    ]
)

# Field productor
cc_library(
    name = 'dissector',
    srcs = 'dissector.cc',
    deps = [
        '//twister/common/base:twister_flags',
        '//common/encoding:encoding',
    ],
)

cc_library(
    name = 'pb_dissector',
    srcs = 'pb_dissector.cc',
    deps = [
        ':block',
        ':dissector',
        ':table_options_proto',
        ':tablet_schema_proto',
        '//twister/common/base:twister_flags',
        '//twister/common/base:data_holder',
        '//twister/utils:proto_message',
        '//common/base/string:string',
        '//common/file/recordio:recordio',
        '//common/file:file',
        '//common/file:local_file',
    ],
    link_all_symbols = 1
)

cc_test(
    name = 'pb_dissector_test',
    srcs = 'pb_dissector_test.cc',
    deps = [
        ':pb_dissector',
        '//common/base/string:string',
        '//thirdparty/gtest:gtest'
    ],
    testdata = [
        'testdata/recordfile_one.dat',
        'testdata/recordfile_two.dat'
    ]
)

cc_library(
    name = 'csv_dissector',
    srcs = 'csv_dissector.cc',
    deps = [
        ':block',
        ':dissector',
        ':table_options_proto',
        ':tablet_schema_proto',
        '//twister/common/base:data_holder',
        '//twister/common/base:twister_flags',
        '//twister/utils:proto_message',
        '//common/base/string:string',
        '//common/file:file',
        '//common/file:local_file',
    ],
    link_all_symbols = 1
)

cc_test(
    name = 'csv_dissector_test',
    srcs = 'csv_dissector_test.cc',
    deps = [
        ':csv_dissector',
        '//twister/utils:csv_helper',
        '//common/base/string:string',
        '//thirdparty/gtest:gtest'
    ],
)

cc_library(
    name = 'field_productor',
    srcs = [
        'field_productor.cc'
    ],
    deps = [
        ':pb_dissector',
        ':csv_dissector',
        '//common/base/string:string',
    ]
)

cc_test(
    name = 'field_productor_test',
    srcs = 'field_productor_test.cc',
    deps = [
        ':field_productor',
        '//thirdparty/gmock:gmock',
        '//thirdparty/gtest:gtest'
    ]
)

cc_library(
    name = 'slice_column_reader',
    srcs = 'slice_column_reader.cc',
    deps = [
        ':slice',
        ':tablet_reader',
    ],
)

cc_test(
    name = 'slice_column_reader_test',
    srcs = 'slice_column_reader_test.cc',
    deps = [
        ':slice_column_reader',
        ':table_builder',
        ':test_data',
        '//common/file:local_file',
    ],
    testdata = [
        'testdata/document.proto'
    ]
)

cc_library(
    name = 'pb_assembler',
    srcs = 'pb_assembler.cc',
    deps = [
        ':automaton',
        ':slice_column_reader',
        ':tablet_reader',
        '//common/base/string:string',
        '//twister/utils:proto_message'
    ]
)

cc_test(
    name = 'pb_assembler_test',
    srcs = 'pb_assembler_test.cc',
    deps = [
        ':pb_assembler',
        ':table_builder',
        ':test_data',
        '//common/file:local_file',
    ],
    testdata = [
        'testdata/document.proto',
        'testdata/all_types.proto',
    ]
)

cc_library(
    name = 'automaton',
    srcs = 'automaton.cc',
    deps = [
        '//thirdparty/protobuf:protobuf',
        '//common/base/string:string',
    ]
)

cc_library(
    name = 'slice_scanner',
    srcs = 'slice_scanner.cc',
    deps = [
        ':slice',
    ]
)

cc_test(
    name = 'slice_scanner_test',
    srcs = 'slice_scanner_test.cc',
    deps = [
        ':slice_scanner',
        '//twister/utils:test_helper',
        '//thirdparty/gtest:gtest',
    ]
)

cc_library(
    name = 'tablet_scanner',
    srcs = 'tablet_scanner.cc',
    deps = [
        ':slice',
        ':tablet_reader',
    ]
)

cc_library(
    name = 'io_test_helper',
    srcs = 'io_test_helper.cc',
    deps = [
        ':table_builder',
        '//twister/utils:proto_message',
        '//twister/io/testdata:document_proto'
    ]
)

cc_library(
    name = "short_display_reader",
    srcs = "short_display_reader.cc",
    deps = [
        ':column_metadata_proto',
        ':tablet_reader',
        ':tablet_schema_proto',
        '//common/system/memory:memory',
    ]
)

cc_test(
    name = 'short_display_reader_test',
    srcs = ['short_display_reader_test.cc'],
    deps = [
        ':short_display_reader',
        ':io_test_helper',
        ':table_builder',
        ':table_options_proto',
        '//common/file/recordio:recordio',
        '//twister/io/testdata:document_proto',
        '//thirdparty/gtest:gtest',
    ],
    testdata = [
        'testdata/document.proto',
    ]
)

cc_library(
    name = 'test_data',
    srcs = 'test_data.cc',
    deps = [
        '//twister/utils:proto_message',
        '//common/base/string:string',
        '//common/crypto/random:random',
        '//common/system/time:time',
    ]
)

cc_library(
    name = 'slice',
    srcs = [
        'slice.cc',
    ],
    deps = [
        ':block',
        '//common/base/string:string',
    ]
)

cc_test(
    name = 'slice_test',
    srcs = [
        'slice_test.cc'
    ],
    deps = [
        ':slice',
    ]
)

cc_library(
    name = 'block',
    srcs = [
        'block.cc',
        'block_helper.cc'
    ],
    deps = [
        '//twister/common/base:data_holder',
        '//twister/common/proto:error_code_proto',
        '//common/base/string:string',
        '//thirdparty/glog:glog',
    ],
)

cc_test(
    name = 'block_test',
    srcs = [
        'block_test.cc'
    ],
    deps = [
        ':block',
    ]
)

cc_library(
    name = 'history_info_io',
    srcs = [
        'history_info_io.cc',
    ],
    deps = [
        '//common/base/string:string',
        '//common/file:file',
        '//common/file:local_file',
        '//thirdparty/protobuf:protobuf',
    ]
)

cc_test(
    name = 'history_info_io_test',
    srcs = [
        'history_info_io_test.cc'
    ],
    deps = [
        ':history_info_io',
        '//twister/io/testdata:document_proto',
    ],
    testdata = [
        'testdata/document.proto',
        '//thirdparty/gtest:gtest'
    ]
)

cc_test(
    name = 'slice_scanner_performance_test',
    srcs = [
        'slice_scanner_performance_test.cc'
    ],
    deps = [
        ':block',
        ':slice',
        ':slice_scanner',
        '//common/base/string:string',
        '//common/system/time:time',
        '//thirdparty/perftools:profiler',
    ]
)
