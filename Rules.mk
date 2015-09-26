#
# file: Rules.mk
#
# author: Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# date: 2015-09-26
#

#-----------------------------------------------------------------------------------------------------------------------
# subdirectories
#-----------------------------------------------------------------------------------------------------------------------

SUBDIRECTORIES += source
SUBDIRECTORIES += test

#-----------------------------------------------------------------------------------------------------------------------
# generated headers
#-----------------------------------------------------------------------------------------------------------------------

DISTORTOS_CONFIGURATION_H := $(OUTPUT)include/distortos/distortosConfiguration.h

$(DISTORTOS_CONFIGURATION_H): $(DISTORTOS_CONFIGURATION_MK)
	./scripts/makeDistortosConfiguration.awk "$<" > "$@"

#-----------------------------------------------------------------------------------------------------------------------
# generated headers depend on this Rules.mk, the script that generates them and the selectedConfiguration.mk file
#-----------------------------------------------------------------------------------------------------------------------

$(DISTORTOS_CONFIGURATION_H): Rules.mk scripts/makeDistortosConfiguration.awk selectedConfiguration.mk

#-----------------------------------------------------------------------------------------------------------------------
# final targets
#-----------------------------------------------------------------------------------------------------------------------

ELF := $(ELF) $(OUTPUT)$(PROJECT).elf
HEX := $(HEX) $(OUTPUT)$(PROJECT).hex
BIN := $(BIN) $(OUTPUT)$(PROJECT).bin
DMP := $(DMP) $(OUTPUT)$(PROJECT).dmp
LSS := $(LSS) $(OUTPUT)$(PROJECT).lss

#-----------------------------------------------------------------------------------------------------------------------
# .elf file depends on this Rules.mk
#-----------------------------------------------------------------------------------------------------------------------

$(ELF): Rules.mk

#-----------------------------------------------------------------------------------------------------------------------
# add generated headers and final targets to list of generated files
#-----------------------------------------------------------------------------------------------------------------------

GENERATED := $(GENERATED) $(DISTORTOS_CONFIGURATION_H) $(ELF) $(HEX) $(BIN) $(DMP) $(LSS) $(OUTPUT)$(PROJECT).map

#-----------------------------------------------------------------------------------------------------------------------
# standard footer
#-----------------------------------------------------------------------------------------------------------------------

include footer.mk
