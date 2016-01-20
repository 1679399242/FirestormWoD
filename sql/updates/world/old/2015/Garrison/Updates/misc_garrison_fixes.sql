#34822/Quest - What we need

	REPLACE INTO creature_loot_template VALUE
	(80174, 111907, -100, 1, 0, 1, 2, '');

	REPLACE INTO gameobject_loot_template VALUES
	(230881, 111907, -100, 1, 0, 1, 2, ''),
	(230879, 111907, -100, 1, 0, 1, 2, ''),
	(230880, 111907, -100, 1, 0, 1, 2, '');

#38293/Quest - Somptuous Fur

	REPLACE INTO creature_queststarter VALUE (91025, 38293);
	REPLACE INTO creature_questender VALUES
	(91025, 38293),
	(91034, 38293);

#91034/NPC - Calvo Klyne <Fur Trader>

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91034;
	DELETE FROM creature WHERE id = 91034;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('91034','1330','7004','7210','2','4294967295','0','0','5553.67','4598.79','141.164','5.81351','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91034, 120945, 5815), -- 5 somptuous fur
	(91034, 122549, 5829), -- 60 somptuous fur
	(91034, 122716, 5829),
	(91034, 127724, 5829),
	(91034, 127742, 5829);

#91025/NPC - Dorothy Two

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91025;
	DELETE FROM creature WHERE id = 91025;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('91025','1331','7078','7211','2','1','0','0','1937.66','330.089','91.764','6.21431','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91025, 120945, 5815), -- 5 somptuous fur
	(91025, 122549, 5829), -- 60 somptuous fur
	(91025, 122716, 5829),
	(91025, 127724, 5829),
	(91025, 127742, 5829);

#91404/NPC - Samantha Scarlet <Herb Trader>

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91404;
	DELETE FROM creature WHERE id = 91404;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('91404','1331','7078','7211','2','1','0','0','1938.53','331.616','91.7648','5.90408','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91404, 122710, 5824), -- 60 Starflower
	(91404, 122741, 5827), -- 20 Cerulean Pigments
	(91404, 122553, 5827),
	(91404, 122713, 5827),
	(91404, 127723, 5827),
	(91404, 127728, 5827),
	(91404, 122548, 5827),
	(91404, 127746, 5827),
	(91404, 128409, 5827),
	(91404, 128410, 5827),
	(91404, 128411, 5827),
	(91404, 128412, 5827),
	(91404, 128413, 5827),
	(91404, 128414, 5810), -- 10 Alchemical Catalyst
	(91404, 122599, 5810),
	(91404, 122600, 5810),
	(91404, 122605, 5810),
	(91404, 128160, 5810),
	(91404, 120945, 5296); -- 5 Talador Orchid (5822 seems okay also)

#38296/Quest - Herbs Galore

	REPLACE INTO creature_queststarter VALUE (91404, 38296);
	REPLACE INTO creature_questender VALUES
	(91404, 38296),
	(91031, 38296);

#91031/NPC - Nicholas Mitrik

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91031;
	DELETE FROM creature WHERE id = 91031;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('91031','1330','7004','7210','2','1','0','0','5554.42','4600.68','141.163','5.51585','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91031, 122710, 5824), -- 60 Starflower
	(91031, 122741, 5827), -- 20 Cerulean Pigments
	(91031, 122553, 5827),
	(91031, 122713, 5827),
	(91031, 127723, 5827),
	(91031, 127728, 5827),
	(91031, 122548, 5827),
	(91031, 127746, 5827),
	(91031, 128409, 5827),
	(91031, 128410, 5827),
	(91031, 128411, 5827),
	(91031, 128412, 5827),
	(91031, 128413, 5827),
	(91031, 128414, 5810), -- 10 Alchemical Catalyst
	(91031, 122599, 5810),
	(91031, 122600, 5810),
	(91031, 122605, 5810),
	(91031, 128160, 5810),
	(91031, 120945, 5296); -- 5 Talador Orchid (5822 seems okay also)

#37961/Quest - Bringing the Bass -- Horde

	UPDATE creature_template SET npcflag = 2 WHERE entry = 91072;
	DELETE FROM creature WHERE id = 91072;
--	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('91072','1330','7004','7210','2','1','0','0','5588.78','4602.95','136.588','0.0270831','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
-- add spawns later when jukebox is scripted
	REPLACE INTO creature_queststarter VALUE (91072, 37961);
	REPLACE INTO creature_questender   VALUE (91072, 37961);

	REPLACE INTO gameobject_loot_template VALUE (240568, 122565, -100, 1, 0, 1, 1, '');
	REPLACE INTO gameobject_loot_template VALUE (240575, 122566, -100, 1, 0, 1, 1, '');

	REPLACE INTO creature_loot_template VALUE (83988, 122567, -100, 1, 0, 1, 1, '');
	REPLACE INTO creature_loot_template VALUE (83987, 122567, -100, 1, 0, 1, 1, '');
	REPLACE INTO creature_loot_template VALUE (83989, 122567, -100, 1, 0, 1, 1, '');

	REPLACE INTO creature_loot_template VALUE (75815, 122568, -100, 1, 0, 1, 1, '');
	REPLACE INTO creature_loot_template VALUE (86262, 122569, -100, 1, 0, 1, 1, '');

	-- SPAWNS

#38356/Quest - Bringing the Bass -- Alliance

	DELETE FROM creature WHERE id = 91589;
--	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('91589','1331','7078','7211','2','4294967295','0','0','1924.43','310.853','88.9662','3.78067','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
-- add spawns later when jukebox is scripted

	UPDATE creature_template SET npcflag = 2 WHERE entry = 91589;
	REPLACE INTO creature_queststarter VALUE (91589, 38356);
	REPLACE INTO creature_questender   VALUE (91589, 38356);

#34823/Quest - Ogron Live

	UPDATE quest_template SET ExclusiveGroup = 0, prevquestid = 0, nextquestid = 0, NextQuestIdChain = 0 WHERE id = 34823;

#35176/Quest - Keeping it Together

	insert into `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) values('84455','0','0','0','64','0','100','0','0','0','0','0','33','84455','0','0','0','0','0','0','0','0','0','0','0','0','0','Assistant Brightstone kill credit');

#33787/Quest - Fiona s solution

	REPLACE INTO creature_loot_template VALUE (82427, 108396, -100, 1, 1, 1, 1, '');

#33263/Quest - Gloomshade Grove -- TEMPFIX, quest to script

	UPDATE quest_template SET method = 0, flags = flags | 0x00010000 WHERE id = 33263; -- autocomp

#37060 - 37087/Quest - Lost in Transition

	UPDATE gameobject_template SET data17 = 0, data1 = 237039  WHERE entry = 237039;
	UPDATE gameobject_template SET data17 = 0, data1 = 237257  WHERE entry = 237257;
	DELETE FROM gameobject_loot_template WHERE entry IN (237039, 237257);
	INSERT INTO gameobject_loot_template VALUES
	(237039, 118417, -100, 1, 1, 1, 1, ''),
	(237257, 118417, -100, 1, 1, 1, 1, '');

	DELETE FROM gameobject WHERE id IN (237039, 237257);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1900.48','269.985','76.8125','3.50048','0','0','0.983943','-0.178481','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1900.48','267.418','76.9237','4.91027','0','0','0.633802','-0.773495','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1897.34','268.321','76.7488','0.343177','0','0','0.170748','0.985315','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1940.32','251.478','76.6396','6.1669','0','0','0.0581077','-0.99831','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1942.25','251.12','76.6396','0.896882','0','0','0.433561','0.901124','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1949.36','282.863','88.9659','1.49379','0','0','0.679362','0.733803','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1877.37','354.633','81.6616','0.743734','0','0','0.363355','0.931651','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1875.68','280.926','76.6404','1.75297','0','0','0.768495','0.639856','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1876.28','277.315','76.6402','1.69799','0','0','0.750618','0.660737','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1874.13','280.03','76.6398','0.366743','0','0','0.182346','0.983235','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1844.01','295.39','76.6419','4.8121','0','0','0.670989','-0.741467','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1842.03','295.286','76.6403','4.52151','0','0','0.771273','-0.636504','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1832.29','278.046','76.6396','2.56194','0','0','0.958294','0.285784','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1831.55','276.413','76.6397','1.28174','0','0','0.597895','0.801575','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1829.85','256.233','76.6398','0.618082','0','0','0.304145','0.952626','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1825.3','261.531','76.6396','1.26211','0','0','0.589998','0.807405','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1826.26','256.602','76.6396','2.57766','0','0','0.96051','0.278247','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1749.42','283.903','77.4565','5.55432','0','0','0.356421','-0.934326','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1749.57','281.195','77.3862','1.55664','0','0','0.702084','0.712094','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1743.74','228.148','73.5245','4.34481','0','0','0.824427','-0.565968','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1784.3','183.993','71.845','3.72435','0','0','0.957849','-0.287271','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1785.89','182.237','71.8461','3.0489','0','0','0.998926','0.0463287','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1785.13','180.458','71.8452','4.41549','0','0','0.803913','-0.594747','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1807.39','216.078','71.2062','1.68624','0','0','0.746721','0.665138','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1807.27','216.167','70.0195','4.97705','0','0','0.607624','-0.794225','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1804.1','219.916','69.9811','4.6786','0','0','0.71895','-0.695062','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1817.48','197.984','71.9859','3.2963','0','0','0.99701','-0.0772789','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1821.05','203.251','71.9861','3.8343','0','0','0.940616','-0.339472','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1840.23','199.213','72.0368','5.03203','0','0','0.585564','-0.810626','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1839.54','201.628','72.0368','4.94957','0','0','0.618481','-0.785799','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1835.63','206.656','72.0344','0.170416','0','0','0.085105','0.996372','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1845.3','235.252','76.6218','0.500288','0','0','0.247544','0.968877','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1839.34','238.631','76.6397','1.46633','0','0','0.669223','0.743061','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1841.14','236.387','76.6397','5.17734','0','0','0.525178','-0.850992','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1838.88','241.148','76.6397','5.15377','0','0','0.535167','-0.844746','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1838.88','241.148','76.6397','5.15377','0','0','0.535167','-0.844746','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1859.66','221.166','76.6398','4.16418','0','0','0.872112','-0.489307','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1858.93','219.808','76.6397','3.77932','0','0','0.949592','-0.31349','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1881.27','226.466','76.7161','6.08053','0','0','0.101153','-0.994871','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1880.62','224.608','76.787','2.43235','0','0','0.937778','0.347235','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1879.38','193.739','78.0563','2.38131','0','0','0.928613','0.37105','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237039','1331','7078','7211','2','4294967295','1749.57','281.195','77.3862','1.55664','0','0','0.702084','0.712094','300','0','1','0','0',NULL);

	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5717.17','4516.6','131.843','0.148625','0','0','0.0742443','0.99724','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5516.53','4491.61','135.305','3.52347','0','0','0.981827','-0.18978','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5542.31','4483.46','133.342','5.03536','0','0','0.584213','-0.8116','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5515.78','4489.91','135.405','1.9134','0','0','0.817295','0.576219','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5528.05','4474.39','134.908','3.18026','0','0','0.999813','-0.0193303','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5529.81','4474.17','134.812','4.28767','0','0','0.840258','-0.542187','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5465.74','4453.59','144.651','4.06383','0','0','0.895555','-0.444951','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5424.43','4533.43','139.123','2.72159','0','0','0.97803','0.208463','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5415.01','4536.51','139.123','4.00964','0','0','0.907281','-0.420525','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5426.03','4585.08','136.496','4.59869','0','0','0.74614','-0.665789','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5408.21','4573.71','138.082','4.64582','0','0','0.730247','-0.683183','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5450.5','4617.25','135.099','2.86373','0','0','0.990364','0.138485','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5457','4618.16','135.039','1.30864','0','0','0.608619','0.793462','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5465.78','4618.57','134.606','2.02652','0','0','0.84856','0.529099','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5470.5','4619.98','134.45','1.46496','0','0','0.668715','0.743519','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5630.04','4663.68','142.458','2.05008','0','0','0.854736','0.519063','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5628.64','4659.71','142.458','1.47282','0','0','0.671631','0.740886','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5637.57','4625.73','139.244','4.18637','0','0','0.86663','-0.498951','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5635.86','4624.6','139.258','0.188686','0','0','0.0942029','0.995553','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5580.91','4610.51','136.588','0.797371','0','0','0.388207','0.921572','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5578.01','4574.45','136.586','4.12668','0','0','0.881132','-0.47287','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5567.38','4572.1','136.588','2.5873','0','0','0.96184','0.273611','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5566.13','4570.31','136.616','0.125077','0','0','0.062498','0.998045','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5547.7','4580.69','138.591','4.60732','0','0','0.743263','-0.669','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5549.41','4578.82','138.477','3.33497','0','0','0.995329','-0.0965392','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5541.69','4539.3','131.732','1.14218','0','0','0.540547','0.841313','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5540.4','4539.7','131.77','2.6737','0','0','0.972759','0.231817','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5581.47','4528.64','127.932','1.09898','0','0','0.522251','0.852792','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5582.13','4527.25','127.754','3.79682','0','0','0.946813','-0.321784','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5583.43','4528.81','127.485','2.44593','0','0','0.940115','0.340858','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5582.9','4505','130.084','4.9529','0','0','0.617172','-0.786828','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5583.87','4502.52','130.282','0.5743','0','0','0.28322','0.959055','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5624.71','4503.65','119.27','1.90009','0','0','0.813442','0.581646','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5635.3','4553.22','119.351','1.3621','0','0','0.629607','0.776914','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5635.27','4556.87','119.59','2.56376','0','0','0.958553','0.284914','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5642.69','4560.53','119.316','4.47773','0','0','0.785019','-0.619471','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5611.17','4505.8','119.27','1.55452','0','0','0.701329','0.712837','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5610.97','4472.03','130.189','4.6097','0','0','0.742464','-0.669887','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5604.69','4472.32','130.258','1.44455','0','0','0.661092','0.750305','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5604.65','4470.44','130.367','1.56236','0','0','0.704116','0.710085','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5598.6','4455.98','130.367','3.76694','0','0','0.951514','-0.307604','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5605.74','4465.93','130.371','1.99433','0','0','0.839935','0.542687','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5606.49','4464.14','130.362','1.93149','0','0','0.822474','0.568802','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5627.23','4458.7','130.196','1.24819','0','0','0.584364','0.811491','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5622.15','4454.41','130.191','5.37939','0','0','0.436674','-0.89962','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5613.61','4465.8','130.225','1.09504','0','0','0.520572','0.853818','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5610.93','4463.74','130.261','2.82054','0','0','0.987143','0.159839','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5637.37','4464.79','130.47','6.08232','0','0','0.100264','-0.994961','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5636.57','4457.6','130.906','4.55079','0','0','0.761871','-0.647729','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5631.83','4460.61','130.737','4.1149','0','0','0.883903','-0.467669','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5627.07','4411.51','136.359','0.0111914','0','0','0.00559565','0.999984','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5619.88','4407.57','136.796','3.32165','0','0','0.99595','-0.0899053','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5667.61','4480.93','130.415','0.478496','0','0','0.236972','0.971516','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237257','1330','7004','7210','2','4294967295','5663.94','4482.9','130.368','4.3348','0','0','0.827248','-0.561838','300','0','1','0','0',NULL);

#162367/Spell - Convert to Garrison Resources

	DELETE FROM disables WHERE entry = 162367;

#83858/NPC - Khadgar s Servant

	DELETE FROM creature WHERE id = 83858;
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('83858','1331','7078','7211','2','4294967295','0','0','1862.32','220.471','76.6396','0.666129','300','0','0','37219','0','0','0','0','0','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('83858','1330','7004','7210','2','4294967295','0','0','5621.16','4546.29','119.271','5.62891','300','0','0','37219','0','0','0','0','0','2048','0','0','0','0',NULL);

#34822/Quest - What we Need

	UPDATE gameobject_template SET flags = flags & ~0x00000004, data1 = entry WHERE entry IN (230881, 230879, 230880);
	DELETE FROM gameobject WHERE id IN (230881, 230879, 230880);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7062','1','1','5656.31','4919.1','15.0634','5.73839','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7062','1','1','5742.41','4892.2','30.0262','3.64361','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7062','1','1','5742.94','4898.1','29.7908','0.0978507','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7062','1','1','5686.52','5029.7','31.7383','2.35632','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7257','1','1','5654.38','4933.54','14.4938','5.54686','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7257','1','1','5657.88','5023.83','28.8026','0.0978507','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230880','1116','6720','7062','1','1','5736.98','4899.85','29.355','0.0600323','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230880','1116','6720','7062','1','1','5655.77','4912.72','14.7241','1.22845','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230880','1116','6720','7062','1','1','5723.81','4939.8','29.9174','3.64023','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230880','1116','6720','7257','1','1','5662.62','5022.9','29.0532','2.52414','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230880','1116','6720','7257','1','1','5663.23','4947.1','15.2013','5.67186','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230880','1116','6720','7062','1','1','5730.66','4921.95','28.6963','0.194753','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230879','1116','6720','7062','1','1','5649.41','4926.8','15.1692','5.17388','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230879','1116','6720','7062','1','1','5735.31','4917.07','28.6798','2.92067','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230879','1116','6720','7257','1','1','5661.23','4955.27','13.7948','5.59297','0','0','0','0','120','255','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230880','1116','6720','7062','1','1','5581.97','5040.46','-27.9258','2.17097','0','0','0.88453','0.466484','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230880','1116','6720','7257','1','1','5573.15','5022.64','-7.98583','1.49553','0','0','0.680002','0.73321','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230879','1116','6720','7257','1','1','5642.26','5097.89','4.13948','5.97622','0','0','0.152883','-0.988244','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230879','1116','6720','7257','1','1','5619.41','5110.02','-8.0834','5.51677','0','0','0.3739','-0.927469','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230879','1116','6720','7257','1','1','5596.23','5144.59','-13.3811','6.18828','0','0','0.047436','-0.998874','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7257','1','1','5547.04','5142.34','-28.9418','5.71311','0','0','0.281191','-0.959652','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7257','1','1','5481.82','5138.97','-38.966','0.0425343','0','0','0.0212655','0.999774','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7257','1','1','5426.49','5080.96','-6.99242','0.855421','0','0','0.414789','0.909918','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7257','1','1','5464.09','5048.25','-9.18784','2.48512','0','0','0.946613','0.322372','300','0','1','0','0',NULL);
	insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('230881','1116','6720','7257','1','1','5474.61','5054.72','0.000994467','3.12523','0','0','0.999967','0.00818263','300','0','1','0','0',NULL);

#35174/Quest - Pale Moonlight

	UPDATE creature SET phasemask = 4294967295 WHERE id = 79205;

#91404/NPC - Samantha Scarlet

	DELETE FROM creature WHERE id = 91404;

#91025/NPC - Dorothy Two

	DELETE FROM creature WHERE id = 91025;

#91034/NPC - Calvo Klyne

	DELETE FROM creature WHERE id = 91034;

#91031/NPC - Nicholas Mitrik

	DELETE FROM creature WHERE id = 91031;

#91483/NPC - Fen Tao

	UPDATE creature_template SET scriptname = "npc_ashran_fen_tao", npcflag = npcflag | 1, gossip_menu_id = 90007 WHERE entry = 91483;
	DELETE FROM gossip_menu WHERE entry IN (90007, 90008);
	INSERT INTO gossip_menu VALUES
	(90007, 92005),
	(90008, 92006);
	DELETE FROM npc_text WHERE ID IN (92005, 92006);
	INSERT INTO npc_text (ID, text0_0, text0_1) VALUES
	(92005,
	"I have heard stories of your glorious deeds in Pandaria! It's an honor to finally meet you in person!",
	"I have heard stories of your glorious deeds in Pandaria! It's an honor to finally meet you in person!"
	),
	(92006,
	"My name is Fen Tao, and I am from Pandaria. I have spent many days traveling to these lands. Why you ask? We are a family of helpful people, and you look like someone that could use some help. Just let me know how I can be of service to you.",
	"My name is Fen Tao, and I am from Pandaria. I have spent many days traveling to these lands. Why you ask? We are a family of helpful people, and you look like someone that could use some help. Just let me know how I can be of service to you."
	);
	DELETE FROM gossip_menu_option WHERE menu_id = 92005;
	INSERT INTO gossip_menu_option (menu_id, option_text) VALUE (92005, "Why are you here?");

#14112/Trinity String

	UPDATE trinity_string SET content_default = "|3ADF00FF+$1i Timber |r" WHERE entry = 14112;