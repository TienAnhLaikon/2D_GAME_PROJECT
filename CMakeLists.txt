#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME SoulSlayer)

project(${APP_NAME})

if(XCODE)
    if(NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET (CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif()
endif()

if(NOT DEFINED BUILD_ENGINE_DONE) # to test SoulSlayer into root project
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)
set(RAPIDJSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/json)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE

     #Scene
     Classes/AppDelegate.cpp
     Classes/Audio/AudioEngine.cpp
     Classes/Engine/Engine.cpp
     Classes/Game/Game.cpp
     
     #locale
     Classes/Localization/LocalizationManager.cpp

     #Map
     Classes/Map/Map.cpp

     #Tutorial
     Classes/TutorialManager/TutorialManager.cpp
	
     #CoinManager
     Classes/CoinManager/CoinManager.cpp

     #Iventory
     Classes/Equipment/Weapon/ChippedSword.cpp
     Classes/Equipment/Weapon/LongSword.cpp
     Classes/Equipment/Weapon/HunterSword.cpp
     Classes/Equipment/Weapon/SoulSlayerSword.cpp
     Classes/Equipment/Weapon/SwordOfKnight.cpp
     Classes/Equipment/Ring/ChippedRing.cpp
     Classes/Equipment/Ring/HunterRing.cpp
     Classes/Equipment/Ring/SoulRing.cpp
     Classes/Equipment/Shield/ChippedShield.cpp
     Classes/Equipment/Shield/KnightShield.cpp
     Classes/Equipment/Shield/SoulShield.cpp
     Classes/Equipment/Shoe/ChippedShoe.cpp
     Classes/Equipment/Shoe/Sneaker.cpp
     Classes/Equipment/Shoe/SoulShoe.cpp
     Classes/Equipment/Armor/ChippedArmor.cpp
     Classes/Equipment/Armor/HunterArmor.cpp
     Classes/Equipment/Armor/SoulArmor.cpp
     Classes/Equipment/Armor/ArmorOfKnight.cpp
     Classes/Equipment/Helmet/ChippedHelmet.cpp
     Classes/Equipment/Helmet/HelmetOfKnight.cpp
     Classes/Equipment/Helmet/SoulHelmet.cpp
     Classes/Equipment/EquipmentFactory.cpp
     Classes/Equipment/BaseEquipment.cpp
     Classes/Inventory/Inventory.cpp
     Classes/Inventory/InventoryNode.cpp
     Classes/Inventory/UpgradeInventory.cpp
     Classes/Inventory/SubInventory.cpp

        #Character
     Classes/BaseCharacter/BaseCharacter.cpp
     #+ NormalMonster
     Classes/Enemies/Enemy.cpp
     Classes/Enemies/Boss/BossFactory.cpp
     Classes/Enemies/NormalMonster/NormalMonsterFactory.cpp
     #- NormalMonster
     Classes/Enemies/NormalMonster/ShadowSentinels.cpp
     Classes/Enemies/NormalMonster/Skeleton.cpp
     Classes/Enemies/NormalMonster/Slime.cpp
     Classes/Enemies/NormalMonster/BabySpider.cpp
     Classes/Enemies/NormalMonster/Angelos.cpp

     Classes/Enemies/NormalMonster/NormalMonster.cpp
     #- Boss
     Classes/Enemies/Boss/HellBeast.cpp
     Classes/Enemies/Boss/Okyanus.cpp
     Classes/Enemies/Boss/Paradiso.cpp
     Classes/Enemies/Boss/Boss.cpp
     #+ Player
     Classes/Player/Player.cpp

     #+NPC
     Classes/NPC/NPC.cpp
     Classes/NPC/NPCFactory.cpp
     Classes/NPC/Robert.cpp
     Classes/NPC/Durin.cpp
     Classes/NPC/ThienAn.cpp
	
     # Quest
	  Classes/Quest/QuestManager.cpp
	  Classes/Quest/Quest.cpp
     #State
     #+ Normal Monster State
     Classes/State/NormalMonsterState/NormalMonsterAttack.cpp
     Classes/State/NormalMonsterState/NormalMonsterDead.cpp
     Classes/State/NormalMonsterState/NormalMonsterIdle.cpp
     Classes/State/NormalMonsterState/NormalMonsterWalk.cpp
     #+ Player State
     Classes/State/PlayerState/PlayerAttack.cpp
     Classes/State/PlayerState/PlayerDead.cpp
     Classes/State/PlayerState/PlayerIdle.cpp
     Classes/State/PlayerState/PlayerSelect.cpp
     Classes/State/PlayerState/PlayerWalk.cpp
     #+ Boss State
     Classes/State/BossState/BossAttack.cpp
     Classes/State/BossState/BossDead.cpp
     Classes/State/BossState/BossGetHit.cpp
     Classes/State/BossState/BossIdle.cpp
     Classes/State/BossState/BossRoar.cpp
     Classes/State/BossState/BossRun.cpp
     Classes/State/BossState/BossWalk.cpp
     Classes/State/BossState/BossAngry.cpp
     Classes/State/BossState/BossAngry.cpp
     Classes/State/BossState/BossHealing.cpp

     #+ NPC State
     Classes/State/NPCState/NPCIdle.cpp
     Classes/State/NPCState/NPCTalking.cpp

     # UserInterface
     Classes/UserInterface/InGame/Joystick.cpp
     Classes/UserInterface/InGame/SkillButton.cpp
     Classes/UserInterface/InGame/InventoryButton.cpp
     Classes/UserInterface/InGame/UpgradeInventoryButton.cpp
     Classes/UserInterface/InGame/TalentButton.cpp
     Classes/UserInterface/InGame/Minimap.cpp
     Classes/UserInterface/InGame/TalentCarousel.cpp
     Classes/UserInterface/InGame/InGameUI.cpp
     Classes/UserInterface/InGame/JoystickSkill.cpp
     Classes/UserInterface/InGame/HealthBar.cpp
     Classes/UserInterface/InGame/ExpBar.cpp
     Classes/UserInterface/InGame/UpgradeProgressBar.cpp 
     Classes/UserInterface/InGame/TalentProgressBar.cpp 
     Classes/UserInterface/InGame/QuestButton.cpp
     Classes/UserInterface/InGame/HamburgerMenuButton.cpp
     Classes/UserInterface/InGame/EquipmentDetails.cpp
     Classes/UserInterface/InGame/ItemDetails.cpp
     Classes/UserInterface/InGame/Scrollview.cpp
     Classes/UserInterface/InGame/LevelUpPopup.cpp
     Classes/UserInterface/InGame/SelectMapButton.cpp
     Classes/UserInterface/InGame/TeleportEffect.cpp
     Classes/UserInterface/InGame/Dropdown.cpp
     Classes/UserInterface/InGame/ShowQuestButton.cpp

     
     # SettingsManager
     Classes/SettingsManager/SettingsManager.cpp
	 Classes/SettingsManager/SettingsManagerInPauseMenu.cpp
     Classes/SettingsManager/SettingsData.cpp

	
	
     # LightEffect
     Classes/LightEffect/Effect.cpp
     Classes/LightEffect/EffectSprite.cpp
     Classes/LightEffect/LightEffect.cpp

     # DialogueManager
     Classes/DialogueManager/DialogueManager.cpp
     Classes/NotificationManager/NotificationManager.cpp
     Classes/SettingsManager/HamburgerMenu.cpp

     # Item
     Classes/Item/UpgradeEquipment/Crystal.cpp
     Classes/Item/UpgradeEquipment/Diamond.cpp
     Classes/Item/UpgradeEquipment/Topaz.cpp
     Classes/Item/UpgradeEquipment/Emerald.cpp


     Classes/Item/BaseItem.cpp
     Classes/Item/PotionManager.cpp
     Classes/Item/ItemFactory.cpp
     Classes/Item/Potion/HealthPotion.cpp
     Classes/Item/Potion/ManaPotion.cpp
     Classes/Item/Potion/SpeedPotion.cpp
     Classes/Item/Potion/ScrollPaper.cpp
     Classes/Item/Comon/Gold.cpp
     
     # ItemManager
     Classes/ItemManager/ItemManager.cpp

     # Player Skill
     Classes/Skills/SkillTree.cpp
     Classes/Skills/SkillBase.cpp
     Classes/Skills/SkillFactory.cpp
     #+ FireSkill
     Classes/Skills/Fire/Fire.cpp
     Classes/Skills/Fire/FireBall.cpp
     Classes/Skills/Fire/FireCastA.cpp
     Classes/Skills/Fire/FireCastB.cpp
     Classes/Skills/Fire/FireCombust.cpp
     Classes/Skills/Fire/FireConflagration.cpp
     Classes/Skills/Fire/FireEruption.cpp
     Classes/Skills/Fire/FireFlare.cpp
     Classes/Skills/Fire/FireProminence.cpp
     Classes/Skills/Fire/FireScorch.cpp
     Classes/Skills/Fire/FireSear.cpp
     #+ ThunderSkill
     Classes/Skills/Thunder/Thunder.cpp
     Classes/Skills/Thunder/ThunderBolt.cpp
     Classes/Skills/Thunder/ThunderCastA.cpp
     Classes/Skills/Thunder/ThunderCastB.cpp
     Classes/Skills/Thunder/ThunderCrackle.cpp
     Classes/Skills/Thunder/ThunderCurrent.cpp
     Classes/Skills/Thunder/ThunderDynamo.cpp
     Classes/Skills/Thunder/ThunderFulminationA.cpp
     Classes/Skills/Thunder/ThunderFulminationB.cpp
     Classes/Skills/Thunder/ThunderShock.cpp
     Classes/Skills/Thunder/ThunderSpark.cpp
     Classes/Skills/Thunder/ThunderVoltage.cpp

     #Boss Skills
     Classes/BossSkills/BossSkillFactory.cpp
     #+  HellBeastSkills
     Classes/BossSkills/HellBeastSkill/Fortify.cpp
     Classes/BossSkills/HellBeastSkill/Unleash.cpp
     Classes/BossSkills/HellBeastSkill/MultiFireBall.cpp
     Classes/BossSkills/HellBeastSkill/BombRainbow.cpp
     Classes/BossSkills/HellBeastSkill/Summon.cpp
     #+  OkyanusSkills
     Classes/BossSkills/OkyanusSkill/Cure.cpp
     Classes/BossSkills/OkyanusSkill/Burst.cpp

	
#WorkingWithData
     Classes/WorkingWithData/JSON/jsonManager.cpp
     Classes/WorkingWithData/SQLITE/sqliteManager.cpp

     #Scenes
     Classes/Scenes/SplashScene.cpp
     Classes/Scenes/MainMenuScene.cpp
     Classes/Scenes/HowToPlayScene.cpp
     Classes/Scenes/LoadingScene.cpp
     Classes/Scenes/PauseMenuScene.cpp 
     Classes/Scenes/ScenesManager.cpp
     Classes/Scenes/TutorialScene.cpp
     Classes/Scenes/DiedLayer.cpp
     Classes/Scenes/SelectMapLayer.cpp
     Classes/Scenes/AnimaticScene.cpp

	#Observer
	Classes/Observer/Observer.h
	Classes/Observer/Subject.h
     )
list(APPEND GAME_HEADER

     #Scene
     Classes/AppDelegate.h
     Classes/Audio/AudioEngine.h
     Classes/Engine/Engine.h
     Classes/Game/Game.h
     #locale
     Classes/Localization/LocalizationManager.h
     
     #Map
     Classes/Map/Map.h
     
     #Tutorial
     Classes/TutorialManager/TutorialManager.h

     #CoinManager
     Classes/CoinManager/CoinManager.h

    #Iventory
     Classes/Equipment/Weapon/ChippedSword.h
     Classes/Equipment/Weapon/LongSword.h
     Classes/Equipment/Weapon/HunterSword.h
     Classes/Equipment/Weapon/SoulSlayerSword.h
     Classes/Equipment/Weapon/SwordOfKnight.h
     Classes/Equipment/Ring/ChippedRing.h
     Classes/Equipment/Ring/HunterRing.h
     Classes/Equipment/Ring/SoulRing.h
     Classes/Equipment/Shield/ChippedShield.h
     Classes/Equipment/Shield/KnightShield.h
     Classes/Equipment/Shield/SoulShield.h
     Classes/Equipment/Shoe/ChippedShoe.h
     Classes/Equipment/Shoe/Sneaker.h
     Classes/Equipment/Shoe/SoulShoe.h
     Classes/Equipment/Armor/ChippedArmor.h
     Classes/Equipment/Armor/HunterArmor.h
     Classes/Equipment/Armor/SoulArmor.h
     Classes/Equipment/Armor/ArmorOfKnight.h
     Classes/Equipment/Helmet/ChippedHelmet.h
     Classes/Equipment/Helmet/HelmetOfKnight.h
     Classes/Equipment/Helmet/SoulHelmet.h
     Classes/Equipment/EquipmentFactory.h
     Classes/Equipment/BaseEquipment.h
     Classes/Inventory/Inventory.h
     Classes/Inventory/InventoryNode.h
     Classes/Inventory/UpgradeInventory.h
     Classes/Inventory/SubInventory.h

     #Character
     Classes/BaseCharacter/BaseCharacter.h
     #+ NormalMonster
     Classes/Enemies/Enemy.h
     Classes/Enemies/Boss/BossFactory.h
     Classes/Enemies/NormalMonster/NormalMonsterFactory.h
     #- NormalMonster
     Classes/Enemies/NormalMonster/ShadowSentinels.h
     Classes/Enemies/NormalMonster/Skeleton.h
     Classes/Enemies/NormalMonster/Slime.h
     Classes/Enemies/NormalMonster/BabySpider.h
     Classes/Enemies/NormalMonster/Angelos.h

     Classes/Enemies/NormalMonster/NormalMonster.h
     #- Boss
     Classes/Enemies/Boss/HellBeast.h
     Classes/Enemies/Boss/Okyanus.h
     Classes/Enemies/Boss/Paradiso.h
     Classes/Enemies/Boss/Boss.h
     #+ Player
     Classes/Player/Player.h


     #+NPC
     Classes/NPC/NPC.h
     Classes/NPC/NPCFactory.h
     Classes/NPC/Robert.h
     Classes/NPC/Durin.h
     Classes/NPC/ThienAn.h

     	# Quest
	Classes/Quest/QuestManager.h
	Classes/Quest/Quest.h
	
     # DialogueManager
     Classes/DialogueManager/DialogueManager.h
    Classes/NotificationManager/NotificationManager.h
	Classes/SettingsManager/HamburgerMenu.h
     
     #State
     #+ Normal Monster State
     Classes/State/NormalMonsterState/NormalMonsterAttack.h
     Classes/State/NormalMonsterState/NormalMonsterDead.h
     Classes/State/NormalMonsterState/NormalMonsterIdle.h
     Classes/State/NormalMonsterState/NormalMonsterWalk.h
     Classes/State/NormalMonsterState/NormalMonsterStateBase.h
     #+ Player State
     Classes/State/PlayerState/PlayerAttack.h
     Classes/State/PlayerState/PlayerDead.h
     Classes/State/PlayerState/PlayerIdle.h
     Classes/State/PlayerState/PlayerSelect.h
     Classes/State/PlayerState/PlayerWalk.h
     Classes/State/PlayerState/PlayerStateBase.h
     #+ Boss State
     Classes/State/BossState/BossAttack.h
     Classes/State/BossState/BossDead.h
     Classes/State/BossState/BossGetHit.h
     Classes/State/BossState/BossIdle.h
     Classes/State/BossState/BossRoar.h
     Classes/State/BossState/BossRun.h
     Classes/State/BossState/BossWalk.h
     Classes/State/BossState/BossAngry.h
     Classes/State/BossState/BossHealing.h
     Classes/State/BossState/BossStateBase.h
     #+ NPC State
     Classes/State/NPCState/NPCIdle.h
     Classes/State/NPCState/NPCTalking.h
     Classes/State/NPCState/NPCStateBase.h

     # UserInterface
     Classes/UserInterface/InGame/Joystick.h
     Classes/UserInterface/InGame/SkillButton.h
     Classes/UserInterface/InGame/InventoryButton.h
     Classes/UserInterface/InGame/UpgradeInventoryButton.h
     Classes/UserInterface/InGame/TalentButton.h
     Classes/UserInterface/InGame/Minimap.h
     Classes/UserInterface/InGame/TalentCarousel.h
     Classes/UserInterface/InGame/InGameUI.h
     Classes/UserInterface/InGame/JoystickSkill.h
     Classes/UserInterface/InGame/HealthBar.h
     Classes/UserInterface/InGame/ExpBar.h
     Classes/UserInterface/InGame/UpgradeProgressBar.h
     Classes/UserInterface/InGame/TalentProgressBar.h
	 Classes/UserInterface/InGame/QuestButton.h
     Classes/UserInterface/InGame/HamburgerMenuButton.h
     Classes/UserInterface/InGame/EquipmentDetails.h
     Classes/UserInterface/InGame/ItemDetails.h
     Classes/UserInterface/InGame/Scrollview.h
     Classes/UserInterface/InGame/LevelUpPopup.h
     Classes/UserInterface/InGame/SelectMapButton.h
     Classes/UserInterface/InGame/TeleportEffect.h
     Classes/UserInterface/InGame/Dropdown.h
     Classes/UserInterface/InGame/ShowQuestButton.h


	
     # LightEffect
     Classes/LightEffect/Effect.h
     Classes/LightEffect/EffectSprite.h
     Classes/LightEffect/LightEffect.h

     # Item
     Classes/Item/UpgradeEquipment/Crystal.h
     Classes/Item/UpgradeEquipment/Diamond.h
     Classes/Item/UpgradeEquipment/Topaz.h
     Classes/Item/UpgradeEquipment/Emerald.h
     Classes/Item/BaseItem.h
     Classes/Item/ItemFactory.h
     Classes/Item/PotionManager.h
     Classes/Item/Potion/HealthPotion.h
     Classes/Item/Potion/ManaPotion.h
     Classes/Item/Potion/SpeedPotion.h
     Classes/Item/Potion/ScrollPaper.h
     Classes/Item/Comon/Gold.h


     # ItemManager
     Classes/ItemManager/ItemManager.h
	
     # SettingsManager
     Classes/SettingsManager/SettingsManager.h
     Classes/SettingsManager/SettingsData.h
     Classes/SettingsManager/HamburgerMenu.h
     Classes/SettingsManager/SettingsManagerInPauseMenu.h

     # Player Skills
     Classes/Skills/SkillTree.h
     Classes/Skills/SkillBase.h
     Classes/Skills/SkillFactory.h
     #+ FireSkill
     Classes/Skills/Fire/Fire.h
     Classes/Skills/Fire/FireBall.h
     Classes/Skills/Fire/FireCastA.h
     Classes/Skills/Fire/FireCastB.h
     Classes/Skills/Fire/FireCombust.h
     Classes/Skills/Fire/FireConflagration.h
     Classes/Skills/Fire/FireEruption.h
     Classes/Skills/Fire/FireFlare.h
     Classes/Skills/Fire/FireProminence.h
     Classes/Skills/Fire/FireScorch.h
     Classes/Skills/Fire/FireSear.h
     #+ ThunderSkill
     Classes/Skills/Thunder/Thunder.h
     Classes/Skills/Thunder/ThunderBolt.h
     Classes/Skills/Thunder/ThunderCastA.h
     Classes/Skills/Thunder/ThunderCastB.h
     Classes/Skills/Thunder/ThunderCrackle.h
     Classes/Skills/Thunder/ThunderCurrent.h
     Classes/Skills/Thunder/ThunderDynamo.h
     Classes/Skills/Thunder/ThunderFulminationA.h
     Classes/Skills/Thunder/ThunderFulminationB.h
     Classes/Skills/Thunder/ThunderShock.h
     Classes/Skills/Thunder/ThunderSpark.h
     Classes/Skills/Thunder/ThunderVoltage.h

     #Boss Skills
     Classes/BossSkills/BossSkillBase.h
     Classes/BossSkills/BossSkillFactory.h
     #+  HellBeastSkills
     Classes/BossSkills/HellBeastSkill/Fortify.h
     Classes/BossSkills/HellBeastSkill/Unleash.h
     Classes/BossSkills/HellBeastSkill/MultiFireBall.h
     Classes/BossSkills/HellBeastSkill/BombRainbow.h
     Classes/BossSkills/HellBeastSkill/Summon.h
     #+  OkyanusSkills
     Classes/BossSkills/OkyanusSkill/Cure.h
     Classes/BossSkills/OkyanusSkill/Burst.h

     #WorkingWithData
     Classes/WorkingWithData/JSON/jsonManager.h
     Classes/WorkingWithData/SQLITE/sqliteManager.h

     #Scenes
     Classes/Scenes/SplashScene.h
     Classes/Scenes/MainMenuScene.h
     Classes/Scenes/HowToPlayScene.h
     Classes/Scenes/LoadingScene.h
     Classes/Scenes/PauseMenuScene.h
     Classes/Scenes/ScenesManager.h
     Classes/Scenes/TutorialScene.h
     Classes/Scenes/DiedLayer.h
     Classes/Scenes/SelectMapLayer.h
     Classes/Scenes/AnimaticScene.h

	#Observer Pattern
	Classes/Observer/Observer.h
	Classes/Observer/Subject.h
     )
     

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
        PRIVATE ${RAPIDJSON_INCLUDE_DIR}
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")

    if(MACOSX)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
    elseif(IOS)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()

    # For code-signing, set the DEVELOPMENT_TEAM:
    #set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "GRLXXXX2K9")
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
    cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()