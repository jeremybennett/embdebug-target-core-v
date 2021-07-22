VINCDIR ::= $(shell pkg-config --variable includedir verilator)

VERILATED_SRC ::= $(VINCDIR)/verilated.cpp
VERILATED_DEPS ::= $(VINCDIR)/verilated.cpp         \
                   $(VINCDIR)/verilatedos.h         \
                   $(VINCDIR)/verilated_imp.h       \
                   $(VINCDIR)/verilated.h           \
                   $(VINCDIR)/verilated_heavy.h     \
                   $(VINCDIR)/verilated_syms.h      \
                   $(VINCDIR)/verilated_sym_props.h \
                   $(VINCDIR)/verilated_config.h

VERILATED_VCD_C_SRC ::= $(VINCDIR)/verilated_vcd_c.cpp
VERILATED_VCD_C_DEPS ::= $(VINCDIR)/verilated_vcd_c.cpp \
                         $(VINCDIR)/verilatedos.h \
                         $(VINCDIR)/verilated.h \
                         $(VINCDIR)/verilated_vcd_c.h \
                         $(VINCDIR)/verilated_trace.h \
                         $(VINCDIR)/verilated_trace_defs.h \
                         $(VINCDIR)/verilated_trace_imp.cpp \
                         $(VINCDIR)/verilated_intrinsics.h

MCU_DIR := $(HOME)/gittrees/openhw/core-v-mcu
MODEL_DIR ?= $(MCU_DIR)/build/openhwgroup.org_systems_core-v-mcu_0/model-lib-verilator
MODEL_LIB ?= $(MODEL_DIR)/obj_dir/Vcore_v_mcu_wrapper__ALL.a

CXX := g++
CPPFLAGS := $(shell pkg-config --cflags verilator) -I$(MODEL_DIR)/obj_dir \
            -I../vendor/cxxopts-3.0.0rc/include
CXXFLAGS := -Os -g3
LDFLAGS :=

OBJS = Args.o Dmi.o DtmJtag.o Tap.o VSim.o testbench.o

.PHONY: all
all: boot/boot_code.cde testbench.exe

testbench.exe: $(OBJS) verilated.o verilated_vcd_c.o $(MODEL_LIB)
	$(CXX) $(LDFLAGS) -o $@ $^

verilated.o: $(VERILATED_SRC) $(VERILATED_DEPS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $(VERILATED_SRC)

verilated_vcd_c.o: $(VERILATED_VCD_C_SRC) $(VERILATED_VCD_C_DEPS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $(VERILATED_VCD_C_SRC)

boot/boot_code.cde:
	$(RM) $@
	ln -s $(MCU_DIR)/sim/boot/boot_code.cde $@

# Header dependencies
testbench.o: Args.h Dmi.h DtmJtag.h IDtm.h
Args.o: Args.h
Dmi.o: Dmi.h
DtmJtag.o: IDtm.h DtmJtag.h Tap.h
Tap.o: Tap.h VSim.h
VSim.o: VSim.h

# Object dependencies
testbench.o: testbench.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

Args.o: Args.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
Dmi.o: Dmi.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
DtmJtag.o: DtmJtag.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
Tap.o: Tap.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
VSim.o: VSim.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(OBJS) testbench.exe verilated.o verilated_vcd_c.o

.PHONY: distclean
distclean: clean
	$(RM) *.vcd *.VCD *.log
