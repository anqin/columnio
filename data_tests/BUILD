cc_test(
    name = 'slice_test',
    srcs = ['slice_test.cc'],
    deps = [
        '//common/file:local_file',
        '//twister/io/testdata:document_proto',
        '//twister/io:slice_writer',
        '//twister/io:io_test_helper',
        '//twister/io:tablet_reader',
        '//thirdparty/gtest:gtest',
    ],
    testdata = [
        'testdata/document.proto',
    ]
)

cc_test(
    name = 'io_data_operation_test',
    srcs = [
        'io_data_operation_test.cc'
    ],
    deps = [
        '//common/file:local_file',
        '//common/system/time:time',
        '//twister/io/testdata:document_proto',
        '//twister/compiler/parser:select_stmt_proto',
        '//twister/io:io',
        '//twister/io:io_test_helper',
        '//twister/io:pb_assembler',
        '//twister/io:table_builder',
        '//twister/utils:create_table_tool',
        '//twister/utils:create_table_mapreduce',
        '//twister/utils:filename_tool',
        '//twister/utils:proto_message',
    ],
    testdata = [
        'testdata/document.proto',
        'anthonyqin_private_key.dat',
    ],
    heap_check = 'local',
)
