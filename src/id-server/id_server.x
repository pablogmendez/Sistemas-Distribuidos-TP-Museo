enum ids_response_type {
    IDS_SUCCESS = 0,
    IDS_ERROR   = 1
};

union ids_response switch (ids_response_type type) {
 case IDS_SUCCESS:
    long id;
 default:
    void;
};

program ID_SERVER {
     version ID_SERVER_VERSION {
        ids_response OBTENER_ID()      = 1;
        void         DEVOLVER_ID(long) = 2;
      } = 1;
} = 0x2E8A16D6;
