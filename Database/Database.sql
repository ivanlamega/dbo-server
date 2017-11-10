-- MySQL dump 10.13  Distrib 5.6.23, for Win64 (x86_64)
--
-- Host: localhost    Database: dbodb
-- ------------------------------------------------------
-- Server version	5.6.24-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `account`
--

DROP TABLE IF EXISTS `account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `account` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `userName` varchar(20) NOT NULL,
  `passWord` varchar(20) NOT NULL,
  `AcLevel` int(11) NOT NULL DEFAULT '0',
  `AllowedRace` int(11) NOT NULL DEFAULT '255',
  `LastServerID` int(11) NOT NULL DEFAULT '255',
  `State` int(11) NOT NULL DEFAULT '0',
  `isGameMaster` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `blacklist`
--

DROP TABLE IF EXISTS `blacklist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `blacklist` (
  `pkID` bigint(20) unsigned NOT NULL,
  `owner_id` bigint(20) unsigned NOT NULL,
  `target_id` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`pkID`,`owner_id`),
  UNIQUE KEY `pkID_UNIQUE` (`pkID`),
  KEY `fk_block_owneridx` (`owner_id`),
  CONSTRAINT `fk_block_ownerid` FOREIGN KEY (`owner_id`) REFERENCES `character` (`ID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `character`
--

DROP TABLE IF EXISTS `character`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `character` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `AccID` bigint(20) unsigned NOT NULL DEFAULT '0',
  `GuildID` bigint(20) NOT NULL DEFAULT '0',
  `ServerID` int(11) NOT NULL DEFAULT '0',
  `Name` varchar(20) NOT NULL,
  `Race` int(11) NOT NULL DEFAULT '0',
  `Class` int(11) NOT NULL DEFAULT '0',
  `Gender` int(11) NOT NULL DEFAULT '0',
  `Face` int(11) NOT NULL DEFAULT '0',
  `Hair` int(11) NOT NULL DEFAULT '0',
  `HairColor` int(11) NOT NULL DEFAULT '0',
  `SkinColor` int(11) NOT NULL DEFAULT '0',
  `Level` int(11) NOT NULL DEFAULT '1',
  `CurExp` bigint(20) NOT NULL DEFAULT '0',
  `MapInfoId` bigint(20) NOT NULL DEFAULT '0',
  `worldTblidx` bigint(20) NOT NULL DEFAULT '0',
  `worldId` bigint(20) NOT NULL DEFAULT '0',
  `BindType` int(11) NOT NULL DEFAULT '0',
  `bindWorldId` bigint(20) NOT NULL DEFAULT '0',
  `bindObjectTblid` bigint(20) NOT NULL DEFAULT '0',
  `PositionX` float(11,6) NOT NULL DEFAULT '0.000000',
  `PositionY` float(11,6) NOT NULL DEFAULT '0.000000',
  `PositionZ` float(11,6) NOT NULL DEFAULT '0.000000',
  `DirectionX` float(11,6) NOT NULL DEFAULT '0.000000',
  `DirectionY` float(11,6) NOT NULL DEFAULT '0.000000',
  `DirectionZ` float(11,6) NOT NULL DEFAULT '0.000000',
  `Money` bigint(20) NOT NULL DEFAULT '0',
  `MoneyBank` bigint(20) NOT NULL DEFAULT '0',
  `Marking` int(11) NOT NULL DEFAULT '255',
  `Adult` tinyint(1) NOT NULL DEFAULT '0',
  `TutorialFlag` tinyint(1) NOT NULL DEFAULT '0',
  `NeedNameChange` tinyint(1) NOT NULL DEFAULT '0',
  `ToDelete` tinyint(1) NOT NULL DEFAULT '0',
  `ChangeClass` tinyint(1) NOT NULL DEFAULT '0',
  `IsGameMaster` tinyint(1) NOT NULL DEFAULT '0',
  `TutorialHint` bigint(20) NOT NULL DEFAULT '0',
  `Reputation` bigint(20) NOT NULL DEFAULT '0',
  `MudosaPoint` bigint(20) NOT NULL DEFAULT '0',
  `SpPoint` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`AccID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `fk_character_account_idx` (`AccID`),
  CONSTRAINT `fk_character_account_id` FOREIGN KEY (`AccID`) REFERENCES `account` (`ID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `friendlist`
--

DROP TABLE IF EXISTS `friendlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friendlist` (
  `pkID` bigint(20) unsigned NOT NULL,
  `owner_id` bigint(20) unsigned NOT NULL,
  `friend_id` bigint(20) unsigned NOT NULL,
  `toBlackList` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`pkID`,`owner_id`),
  UNIQUE KEY `pkID_UNIQUE` (`pkID`),
  KEY `fk_friendlist_owneridx` (`owner_id`),
  CONSTRAINT `fk_friendlist_ownerid` FOREIGN KEY (`owner_id`) REFERENCES `character` (`ID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `inventory`
--

DROP TABLE IF EXISTS `inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inventory` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `CharID` bigint(20) unsigned NOT NULL,
  `ItemSerialID` bigint(20) unsigned NOT NULL COMMENT 'Unique ID in Client(this field cannot be duplicated)',
  `ItemID` bigint(20) unsigned NOT NULL,
  `Place` int(11) NOT NULL DEFAULT '0',
  `Slot` int(11) NOT NULL DEFAULT '0',
  `Stack` int(11) NOT NULL DEFAULT '0',
  `Rank` int(11) NOT NULL DEFAULT '1',
  `CurDur` int(11) NOT NULL DEFAULT '100',
  `NeedToIdentify` tinyint(1) NOT NULL DEFAULT '0',
  `Grade` int(11) NOT NULL DEFAULT '0',
  `BattleAttribute` int(11) NOT NULL DEFAULT '0',
  `RestrictType` int(11) NOT NULL DEFAULT '0',
  `Maker` varchar(20) DEFAULT '""',
  `Opt1` bigint(20) unsigned NOT NULL DEFAULT '0',
  `Opt2` bigint(20) unsigned NOT NULL DEFAULT '0',
  `DurationType` int(11) NOT NULL DEFAULT '0',
  `UseStartTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `UseEndTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`ID`,`CharID`,`ItemSerialID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `fk_inventory_character_idx` (`CharID`),
  KEY `fk_inventory_itemserialidx` (`ItemSerialID`),
  CONSTRAINT `fk_inventory_character_id` FOREIGN KEY (`CharID`) REFERENCES `character` (`ID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `online`
--

DROP TABLE IF EXISTS `online`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `online` (
  `AccountID` bigint(20) unsigned NOT NULL,
  `CharID` bigint(20) unsigned NOT NULL,
  `ServerID` int(11) DEFAULT NULL,
  `ChannelID` int(11) DEFAULT NULL,
  `Handle` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`AccountID`,`CharID`),
  KEY `fk_online_character_idx` (`CharID`),
  KEY `fk_online_account_idx` (`AccountID`),
  CONSTRAINT `fk_online_account_id` FOREIGN KEY (`AccountID`) REFERENCES `account` (`ID`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_online_character_id` FOREIGN KEY (`CharID`) REFERENCES `character` (`ID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `quickslot`
--

DROP TABLE IF EXISTS `quickslot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `quickslot` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `CharID` bigint(20) unsigned NOT NULL,
  `TblID` bigint(20) unsigned NOT NULL,
  `Slot` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `Item` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`CharID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `fk_quickslot_character_id_idx` (`CharID`),
  CONSTRAINT `fk_quickslot_character_id` FOREIGN KEY (`CharID`) REFERENCES `character` (`ID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `skills`
--

DROP TABLE IF EXISTS `skills`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `skills` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `CharID` bigint(20) unsigned NOT NULL,
  `SkillID` bigint(20) unsigned NOT NULL,
  `Slot` int(10) unsigned NOT NULL DEFAULT '0',
  `RpBonusType` int(10) unsigned NOT NULL DEFAULT '0',
  `IsRpBonusAuto` int(10) unsigned NOT NULL DEFAULT '0',
  `RemainSec` int(10) unsigned NOT NULL DEFAULT '0',
  `Exp` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`,`CharID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `fk_character_id_idx` (`CharID`),
  CONSTRAINT `fk_character_id` FOREIGN KEY (`CharID`) REFERENCES `character` (`ID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `warfog`
--

DROP TABLE IF EXISTS `warfog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `warfog` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `CharID` bigint(20) unsigned NOT NULL,
  `Object` bigint(20) NOT NULL,
  PRIMARY KEY (`ID`,`CharID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  KEY `pkey_warfog_charid` (`CharID`),
  CONSTRAINT `fk_warfog_charid` FOREIGN KEY (`CharID`) REFERENCES `character` (`ID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping events for database 'dbodb'
--

--
-- Dumping routines for database 'dbodb'
--
/*!50003 DROP PROCEDURE IF EXISTS `spClearOnline` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `spClearOnline`(IN SrvID INT)
BEGIN
	DELETE FROM `online` WHERE `ServerID` = SrvID;
    END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spDeleteOnline` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `spDeleteOnline`(in AccId int, in CharId int)
BEGIN
	DELETE FROM `online` WHERE `AccountID` = AccId AND `CharID` = CharId;
    END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spInsertCharacter` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `spInsertCharacter`( 
	in nAccID bigint, 
	in nServerID int, 
	in strName varchar(20), 
	in nRace int, 
	in nClass int, 
	in nGender int, 
	in nFace int, 
	in nHair int, 
	in nHairColor int, 
	in nSkinColor int, 
	in nLevel int, 
	in nCurExp bigint, 
	in nMapInfoId bigint, 
	in nworldTblidx bigint, 
	in nworldId bigint, 
	in nBindType int, 
	in nbindWorldId bigint, 
	in nbindObjectTblid bigint, 
	in fPositionX float(11,6), 
	in fPositionY float(11,6), 
	in fPositionZ float(11,6), 
	in fDirectionX float(11,6), 
	in fDirectionY float(11,6), 
	in fDirectionZ float(11,6), 
	in nMoney bigint, 
	in nMoneyBank bigint, 
	in nMarking int, 
	in nAdult int, 
	in nTutorialFlag int, 
	in nNeedNameChange int, 
	in nToDelete int, 
	in nChangeClass int, 
	in nIsGameMaster int, 
	in nTutorialHint bigint, 
	in nReputation bigint, 
	in nMudosaPoint bigint, 
	in nSpPoint int)
BEGIN
	INSERT INTO `character` 
	(`AccID`, 
	`ServerID`, 
	`Name`, 
	`Race`, 
	`Class`, 
	`Gender`, 
	`Face`, 
	`Hair`, 
	`HairColor`, 
	`SkinColor`, 
	`Level`, 
	`CurExp`, 
	`MapInfoId`, 
	`worldTblidx`, 
	`worldId`, 
	`BindType`, 
	`bindWorldId`, 
	`bindObjectTblid`, 
	`PositionX`, 
	`PositionY`, 
	`PositionZ`, 
	`DirectionX`, 
	`DirectionY`, 
	`DirectionZ`, 
	`Money`, 
	`MoneyBank`, 
	`Marking`, 
	`Adult`, 
	`TutorialFlag`, 
	`NeedNameChange`, 
	`ToDelete`, 
	`ChangeClass`, 
	`IsGameMaster`, 
	`TutorialHint`, 
	`Reputation`, 
	`MudosaPoint`, 
	`SpPoint`
	)
	VALUES
	(nAccID, 
	nServerID, 
	strName, 
	nRace, 
	nClass, 
	nGender, 
	nFace, 
	nHair, 
	nHairColor, 
	nSkinColor, 
	nLevel, 
	nCurExp, 
	nMapInfoId, 
	nworldTblidx, 
	nworldId, 
	nBindType, 
	nbindWorldId, 
	nbindObjectTblid, 
	fPositionX, 
	fPositionY, 
	fPositionZ, 
	fDirectionX, 
	fDirectionY, 
	fDirectionZ, 
	nMoney, 
	nMoneyBank, 
	nMarking, 
	nAdult, 
	nTutorialFlag, 
	nNeedNameChange, 
	nToDelete, 
	nChangeClass, 
	nIsGameMaster, 
	nTutorialHint, 
	nReputation, 
	nMudosaPoint, 
	nSpPoint
	);
    END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spInsertItem` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `spInsertItem`(IN nItemID BIGINT,
	IN nCharID BIGINT,
	IN nPlace INT,
	IN nSlot INT,
	IN nStack INT,
	IN nRank INT,
	IN nCurDur INT,
	IN nNeedToIdentify INT,
	IN nGrade INT,
	IN nBattleAttribute INT,
	IN nRestrictType INT,
	IN nMaker VARCHAR(20),
	IN nOpt1 BIGINT,
	IN nOpt2 BIGINT,
	IN nDurationType INT, IN nSerialID BIGINT)
BEGIN
	INSERT INTO `inventory` 
	(`ItemID`, 
	`CharID`, 
	`Place`, 
	`Slot`, 
	`Stack`, 
	`Rank`, 
	`CurDur`, 
	`NeedToIdentify`, 
	`Grade`, 
	`BattleAttribute`, 
	`RestrictType`, 
	`Maker`, 
	`Opt1`, 
	`Opt2`, 
	`DurationType`,
   `ItemSerialID`)
	VALUES
	(nItemID,
	nCharID,
	nPlace,
	nSlot,
	nStack,
	nRank,
	nCurDur,
	nNeedToIdentify,
	nGrade,
	nBattleAttribute,
	nRestrictType,
	nMaker,
	nOpt1,
	nOpt2,
	nDurationType,
  nSerialID);
	SELECT LAST_INSERT_ID() AS `LastID` LIMIT 1;
    END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spInsertOnline` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `spInsertOnline`(
		in AccId bigint,
		in CharId bigint,
		in SrvId int,
		in ChanId bigint,
		in Handle bigint)
BEGIN
	INSERT INTO `online` 
	(`AccountID`, 
	`CharID`, 
	`ServerID`, 
	`ChannelID`, 
	`Handle`
	)
	VALUES
	(AccId, 
	 CharId, 
	 SrvId, 
	 ChanId, 
	 Handle
	);
    END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spQueryInsertItem` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `spQueryInsertItem`(IN nItemID BIGINT,
	IN nCharID BIGINT,
	IN nPlace INT,
	IN nSlot INT,
	IN nStack INT,
	IN nRank INT,
	IN nCurDur INT,
	IN nNeedToIdentify INT,
	IN nGrade INT,
	IN nBattleAttribute INT,
	IN nRestrictType INT,
	IN nMaker VARCHAR(20),
	IN nOpt1 BIGINT,
	IN nOpt2 BIGINT,
	IN nDurationType INT, IN nItemSerialID BIGINT)
BEGIN
	INSERT INTO `inventory` 
	(`ItemID`, 
	`CharID`, 
	`Place`, 
	`Slot`, 
	`Stack`, 
	`Rank`, 
	`CurDur`, 
	`NeedToIdentify`, 
	`Grade`, 
	`BattleAttribute`, 
	`RestrictType`, 
	`Maker`, 
	`Opt1`, 
	`Opt2`, 
	`DurationType`,
  `ItemSerialID`)
	VALUES
	(nItemID,
	nCharID,
	nPlace,
	nSlot,
	nStack,
	nRank,
	nCurDur,
	nNeedToIdentify,
	nGrade,
	nBattleAttribute,
	nRestrictType,
	nMaker,
	nOpt1,
	nOpt2,
	nDurationType,
  nItemSerialID);
    END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spUpdateDirection` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `spUpdateDirection`(IN CharID BIGINT, IN DirX FLOAT(11,6), IN DirY FLOAT(11,6), IN DirZ FLOAT(11,6))
BEGIN
	UPDATE `character` SET `DirectionX` = DirX, `DirectionY` = DirY, `DirectionZ` = DirZ WHERE `ID` = CharID;
    END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `spUpdatePosition` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `spUpdatePosition`(in CharID BIGINT, IN PosX float(11,6), in PosY float(11,6), in PosZ float(11,6))
BEGIN
	update `character` set `PositionX` = PosX, `PositionY` = PosY,`PositionZ` = PosZ where `ID` = CharID;
    END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-07-23 23:07:21
