set(srcs "proj1.cpp"
	)

idf_component_register(SRCS "${srcs}"
		INCLUDE_DIRS . ../components/esp-etl/etl/include/ ../components/libesp
	)
