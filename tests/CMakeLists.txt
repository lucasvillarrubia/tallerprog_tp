target_sources(taller_tests
    PRIVATE
    # .cpp files
    # foo.cpp
    test_client_protocol.cpp
    client_to_server_tests.cpp
    PUBLIC
    # .h files
    socket_interface.h
    test_acceptor_thread.h
    mock_socket.h
    )
