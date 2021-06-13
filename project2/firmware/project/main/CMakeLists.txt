set(srcs "./app.cpp"
	"./menus/calibration_menu.cpp"
	"./menus/menu_state.cpp"
	"./menus/appbase_menu.cpp"
	"./menus/gui_list_processor.cpp"
	"./menus/number_menu.cpp"
	"./proj2.cpp"
)

idf_component_register(SRCS "${srcs}"
		INCLUDE_DIRS ../components/esp-etl/etl/include/ ../components/libesp
		)
