cmake_minimum_required(VERSION 3.9)
project(BossMonster VERSION 1.0.1 DESCRIPTION "mylib description")

set(CMAKE_CXX_STANDARD 11)

IF(WIN32)
	execute_process(COMMAND "symlinks.bat")
ELSE(WIN32)
	execute_process(COMMAND bash "symlinks.sh")
ENDIF(WIN32)

add_library(
        BossMonster SHARED
        mandatory.cpp
        Entities/AEntity.hh
        Entities/AEntity.cpp
        Entities/AEntityShooter.cpp
        Entities/AEntityShooter.hh
        Entities/ALivingEntity.cpp
        Entities/ALivingEntity.hh
        Entities/CollideRect.hh
        Entities/CollideRect.cpp
        Entities/SpawnableEntity.hh
        Entities/SpawnableEntity.cpp
        Entities/Vector2.hh
        BossMonster.cpp BossMonster.hh
        Entities/AEnemy.cpp
        Entities/AEnemy.hh
)