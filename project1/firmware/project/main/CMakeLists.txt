set(srcs "proj1.cpp" "mywifihandler.cpp"
	)

idf_component_register(SRCS "${srcs}"
		INCLUDE_DIRS . ../components/esp-etl/etl/include/ ../components/libesp
		EMBED_TXTFILES "certs/cacert.pem" "certs/prvtkey.pem"
	)
