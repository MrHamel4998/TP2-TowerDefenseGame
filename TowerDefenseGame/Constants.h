#pragma once

static const int SCREEN_WIDTH = 1680;
static const int SCREEN_HEIGHT = 1050;

enum TowerType { King, Archer, Mage, Knight };
enum ProjectileType { Arrow, Blast, Fireball };
enum SpellType { SacredLight, Plague };
enum Maps { Map1, Map2, Map3 };
enum EventType { DemonKilled, DemonDamageTaken, TowerActivated, TowerDeactivated, SpellCast, WaveFinished };