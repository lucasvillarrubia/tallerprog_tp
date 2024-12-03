target_sources(taller_tests
    PRIVATE
    # .cpp files
    # foo.cpp
    server_to_client_tests.cpp
    client_to_server_tests.cpp
    main.cpp
    PUBLIC
    # .h files
    socket_interface.h
    test_acceptor_thread.h
    mock_socket.h
    protocol_test.h
    )
