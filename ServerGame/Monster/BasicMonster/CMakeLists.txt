cmake_minimum_required(VERSION 3.9)
project(BasicMonster VERSION 1.0.1 DESCRIPTION "mylib description")

set(CMAKE_CXX_STANDARD 11)

add_library(
        BasicMonster SHARED
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
        BasicMonster.cpp BasicMonster.hh
        Entities/AEnemy.cpp
        Entities/AEnemy.hh
        Entities/Bullet/ABullet.cpp
        Entities/Bullet/ABullet.hh
        Entities/Bullet/EnemyBullet.cpp Entities/Bullet/EnemyBullet.hh)