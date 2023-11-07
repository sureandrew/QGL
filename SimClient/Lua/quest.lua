
No_Condition = 1
Levelup_Condition = 2
Party_Condition = 3
Quest_Condition = 4
Money_Condition = 7

Main_Quest = 5
Sub_Quest = 6


quests = {
		   		 {
					 		Q_ID = 30001,
							QuestFormat = Main_Quest,
							finish_npc_answerList = { 0, 0, 0, 0, 0 }
				  },
				  
				  {
							Q_ID = 30000,
							FinishCondition = No_Condition,
							QuestFormat = Main_Quest,
							finish_npc_answerList = { 0, 0}
				  },
				  
		   		  {
					 		Q_ID = 30002,
							QuestFormat = Main_Quest,
							finish_npc_answerList = { 0}
				  },
				  
		   		  {
					 		Q_ID = 30802,
							QuestFormat = Main_Quest,
							finish_npc_answerList = { 3 }
				  },
				  
				  {
					 		Q_ID = 30801,
							QuestFormat = Main_Quest,
							finish_npc_answerList = { 2 }
				  },
				  
				  {
					 		Q_ID = 25037,
							QuestFormat = Main_Quest,
							finish_npc_answerList = { 1 }
				  },
				  
				  {
					 		Q_ID = 30800,
							QuestFormat = Main_Quest,
							FinishCondition = Money_Condition,
							ConditionValue = 10000000,
							finish_npc_answerList = { 1 }
				  },
				  
   				  {
							Q_ID = 10000,
       						QuestFormat = Main_Quest,
							finish_npc_answerList = {0},
							FinishCondition = No_Condition
				  },

   				  {
							Q_ID = 10001,
       						QuestFormat = Main_Quest,
							finish_npc_answerList = {0},

				  },
				  
				  {
							Q_ID = 10002,
       						QuestFormat = Main_Quest,
							finish_npc_answerList = { 0}

				  },

   				  {
							Q_ID = 10003,
       						QuestFormat = Main_Quest,
							finish_npc_answerList = { 0}

				  },

				  {
							Q_ID = 10004,
       						QuestFormat = Main_Quest,
							finish_npc_answerList = {0},
							FinishCondition = No_Condition
				  },

   				  {
							Q_ID = 10005,
       						QuestFormat = Main_Quest,
							finish_npc_answerList = {0},
							FinishCondition = No_Condition
				  },

   				  {
							Q_ID = 10006,
       						QuestFormat = Main_Quest,
       						FinishCondition = No_Condition,
							finish_npc_answerList = { 0}
				  },

   				  {
							Q_ID = 10007,
       						QuestFormat = Main_Quest,
       						FinishCondition = No_Condition,
							finish_npc_answerList = {0}
				  },


   				  {
							Q_ID = 10008,
       						QuestFormat = Main_Quest,
       						FinishCondition = No_Condition,
							finish_npc_answerList = {0}
				  },

 				  {
							Q_ID = 10009,
       						QuestFormat = Main_Quest,
       						FinishCondition = Party_Condition,
							finish_npc_answerList = {0}
							-- have to form a party and appointed myself as leader, then talk to the npc
				  },

   				  {
							Q_ID = 10010,
       						QuestFormat = Main_Quest,
							finish_npc_answerList = {0, 0}
				  },

				  {
							Q_ID = 10011,
       						QuestFormat = Main_Quest,
							finish_npc_answerList = { 0, 0} -- cheat item 3300 and then talk to finish npc
				  },

   				  {
							Q_ID = 10012,
       						QuestFormat = Main_Quest,
       						FinishCondition = No_Condition,
							finish_npc_answerList = { 0} -- talk to finish npc
				  },
				  
   				  {
							Q_ID = 10013,
							QuestFormat = Main_Quest,
							FinishCondition = No_Condition,	
							finish_npc_answerList = { 0} -- talk to finish npc
				  },

   				  {
							Q_ID = 10014,
							QuestFormat = Main_Quest,
							FinishCondition = No_Condition,	
							finish_npc_answerList = { 0} -- talk to finish npc
				  },
				  
 				  {
							Q_ID = 10015,
							QuestFormat = Main_Quest,
							FinishCondition = No_Condition,
							finish_npc_answerList = { 0}  -- talk to finish npc
				  },

				  {
							Q_ID = 10016,
							QuestFormat = Main_Quest,
							FinishCondition = No_Condition,
							finish_npc_answerList = { 0}  -- talk to finish npc
				  },

   				  {
							Q_ID = 10017,
							QuestFormat = Main_Quest,
							-- cheat battle win mob 9x10 and the talk to finish npc
							finish_npc_answerList = { 0}
				  },

   				  {
							Q_ID = 10018,
							QuestFormat = Main_Quest,
							finish_npc_answerList = { 0 } -- first talk to target1 NPC and then cheat battlw win target1 NPC (or really win the battle) and finally talk to finish NPC
				  },

   				  {
							Q_ID = 10019,
							QuestFormat = Main_Quest,
							FinishCondition = No_Condition,
							finish_npc_answerList = { 0} -- just jump to finishnpc and talk to him
				  },

 				  {
							Q_ID = 10020,
							QuestFormat = Main_Quest,
							finish_npc_answerList = { 0}, -- just jump to finishnpc and talk to him
							FinishCondition = Levelup_Condition,
							ConditionValue = 10

				  },
				  
 				  {
							Q_ID = 10100,
       						QuestFormat = Main_Quest,
                            FinishCondition = Quest_Condition,
       						ConditionValue = 1,
       						beginNPC = 142,
       						beginNPCLocation = {30401, 16, 51},
       						beginNPC_answerList = {0, 0},
							finish_npc_answerList = {0}
							-- first complete the "join the ganster quest" (QuestID = 30202) and talk to the finished NPC
				  },
				  
				   --------########################## the subquests needed for 10101 #################################------------
				  
				  {
				           Q_ID = 30202,
				           QuestFormat = Sub_Quest,
						   finish_npc_answerList = {0, 0}
				  },
				  
				  ------##################################################################################------------
				  
 				  {
							Q_ID = 10101,
							QuestFormat = Main_Quest,
							FinishCondition = Quest_Condition,
							ConditionValue = 10,
							beginNPC = 142, -- the first one is the npcID, the second one is the mapID
							beginNPCLocation = {30401, 16, 51}, -- 1st is map ID, 2nd and 3rd are x and y respectively
							beginNPC_answerList = {1},
							finish_npc_answerList = { 0}
							-------------complete the quest 10 times below and get back to finish npc and talk to him---------
				  },
				  

				  --------########################## the quests needed for 10101 #################################------------
				  {
				           Q_ID = 30301,
              			   QuestFormat = Sub_Quest,
						   finish_npc_answerList = {1} -- talk to npcID = 142(UID = 304 in map, x,y: 30401, 16, 51)
         				   -- cheat item add 10040 and then talk to finish npc

				  },

                  {
				           Q_ID = 30302,
               			   QuestFormat = Sub_Quest,
						   finish_npc_answerList = {1} -- talk to npcID = 142(map, x,y: 30401, 16, 51)
						   -- cheat battle win and then talk to finish npc

				  },


				  {
				           Q_ID = 30300,
                           QuestFormat = Sub_Quest,
						   finish_npc_answerList = {1} -- talk to npcID = 142(map, x,y: 30401, 16, 51)
         				   -- jump to talk to target ID = 96 first and then talk to finishNPC

				  },

                  ------##################################################################################------------
				  {
				           Q_ID = 10102,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
						   finish_npc_answerList = {0}
				  },
				  
				  {
				           Q_ID = 10103,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10104,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },
				  
				  {
				           Q_ID = 10105,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },
				  
				  {
				           Q_ID = 10106,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10107,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10108,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10109,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10110,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10111,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },
				  
				  {
				           Q_ID = 10112,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10113,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10114,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10115,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10116,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,
                           finish_npc_answerList = {0}
				  },
				  
				  {
				           Q_ID = 10117,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  				  
				  
				  {
				           Q_ID = 10118,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  				  
				  {
				           Q_ID = 10119,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  				  				  
				  			  
				  {
				           Q_ID = 10120,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },
				  
				  {
				           Q_ID = 10121,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10122,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
				  
				  {
				           Q_ID = 10123,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
				  
				  {
				           Q_ID = 10124,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10125,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10126,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10127,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
				  {
				           Q_ID = 10128,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10129,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10130,
                           QuestFormat = Main_Quest,
                           FinishCondition = Levelup_Condition,	
                           ConditionValue = 30,
                           finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10200,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10201,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10202,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },

 
 				  {
				           Q_ID = 10203,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10204,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },
				  
				  
 				  {
				           Q_ID = 10205,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10206,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
				  
 				  {
				           Q_ID = 10207,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10208,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,
                           finish_npc_answerList = {0}
				  },				  
				  
				  
 				  {
				           Q_ID = 10209,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10210,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10211,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10212,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10213,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10214,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10215,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10216,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10217,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10218,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10219,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10220,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,	
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10221,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10222,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10223,
                           QuestFormat = Main_Quest,
                           FinishCondition = No_Condition,
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10224,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },				  

 				  {
				           Q_ID = 10225,
                           QuestFormat = Main_Quest,
                           finish_npc_answerList = {0}
				  },	
				  
 				  {
				           Q_ID = 10226,
                           QuestFormat = Main_Quest,       
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10227,
                           QuestFormat = Main_Quest,
						   FinishCondition = Levelup_Condition,
						   ConditionValue = 30,	
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10300,
                           QuestFormat = Main_Quest,       
                           finish_npc_answerList = {0}
				  },				  
				  
 				  {
				           Q_ID = 10301,
                           QuestFormat = Main_Quest, 
						   FinishCondition = No_Condition,	      
                           finish_npc_answerList = {0}
				  },				  
				  
				  
 				  {
				           Q_ID = 10302,
                           QuestFormat = Main_Quest, 
						   FinishCondition = No_Condition,	      
                           finish_npc_answerList = {0}
				  },				  
				  

 				  {
				           Q_ID = 10303,
                           QuestFormat = Main_Quest, 
						   FinishCondition = No_Condition,	      
                           finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10304,
                           QuestFormat = Main_Quest, 
						   FinishCondition = No_Condition,	      
                           finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10305,
                           QuestFormat = Main_Quest, 
						   finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10306,
                           QuestFormat = Main_Quest, 
						   finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10307,
                           QuestFormat = Main_Quest, 
						   finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10308,
                           QuestFormat = Main_Quest, 
						   FinishCondition = No_Condition,	      
                           finish_npc_answerList = {0}
				  },

  				  {
				           Q_ID = 10309,
                           QuestFormat = Main_Quest, 
						   finish_npc_answerList = {0}
				  },


  				  {
				           Q_ID = 10310,
                           QuestFormat = Main_Quest, 
						   finish_npc_answerList = {0}
				  },


 				  {
				           Q_ID = 10311,
                           QuestFormat = Main_Quest, 
						   FinishCondition = No_Condition,	      
                           finish_npc_answerList = {0}
				  },

 				  {
				           Q_ID = 10312,
                           QuestFormat = Main_Quest, 
                           finish_npc_answerList = {0}
				  },
				  
				  
 				  {
				           Q_ID = 10313,
                           QuestFormat = Main_Quest, 
                           finish_npc_answerList = {0}
				  },
				  
				  				  
 		--		  {
		--		           Q_ID = 10314,
        --                 QuestFormat = Main_Quest,
		--				   FinishCondition = No_Condition,
        --                 finish_npc_answerList = {0}
		--		  }
				  
		
}


function GetQuestTalkOption(iden, idxx)

	for x = 1, table.getn(quests) do

		if quests[x].Q_ID == iden then
			return quests[x].finish_npc_answerList[idxx]
		end
	end
	return 800

end

function GetQuestBeginNPCTalkOption(iden, idxx)

	for x = 1, table.getn(quests) do
		if quests[x].Q_ID == iden then
			return quests[x].beginNPC_answerList[idxx]
		end
	end
	return 800

end

function GetQuestConditionValue(ident)

	for x = 1, table.getn(quests) do
  		if quests[x].Q_ID == ident then
			return quests[x].ConditionValue
		end
	end
	return 500

end




function GetQuestTalkCondition(ident)

	for x = 1, table.getn(quests) do
  		if quests[x].Q_ID == ident then
			return quests[x].FinishCondition
		end
	end
	return 500
end

function GetQuestFormat(ident)

	for x = 1, table.getn(quests) do
  		if quests[x].Q_ID == ident then
			return quests[x].QuestFormat
		end
	end
	return 500
end

function GetQuestBeginNPC(ident)

	for x = 1, table.getn(quests) do
  		if quests[x].Q_ID == ident then
			return quests[x].beginNPC
		end
	end
	return 500
end

function GetQuestBeginNPCMapID(ident)

	for x = 1, table.getn(quests) do
  		if quests[x].Q_ID == ident then
			return quests[x].beginNPCLocation[1] 
		end
	end
	return 500
end

function GetQuestBeginNPCPositionX(ident)

	for x = 1, table.getn(quests) do
  		if quests[x].Q_ID == ident then
			return quests[x].beginNPCLocation[2] 
		end
	end
	return 500
end

function GetQuestBeginNPCPositionY(ident)

	for x = 1, table.getn(quests) do
  		if quests[x].Q_ID == ident then
			return quests[x].beginNPCLocation[3] 
		end
	end
	return 500
end