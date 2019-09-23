/* Generated by Together */

#include <iostream>
#include "GroupServerApp.h"
#include "GameCommon.h"
#include "DBConnect.h"
#include "Team.h"

inline void sendChatLog(Player *ply, GroupServerApp *gsapp, const char *l_word, const char *channel){
	WPacket discordPkt = gsapp->GetWPacket();
	discordPkt.WriteCmd(CMD_PM_DISCORDLOGS);
	discordPkt.WriteString(l_word);
	discordPkt.WriteString(channel);
	gsapp->SendToClient(ply, discordPkt);
}

void	GroupServerApp::CP_GM1SAY(Player *ply,DataSocket *datasock,RPacket &pk)
{
	uShort		l_len;
	cChar	*	l_content	=pk.ReadString(&l_len);
	cChar * Nsrc = "";
	if(!l_content ||l_len >500)return;
	//Modify by sunny.sun 20080821
	if( ply != NULL )
	{
		if(!ply->m_gm)
		{
			//ply->SendSysInfo("你没有该权限.");
			ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00001));
			return;
		}
	}
	WPacket	l_wpk	=GetWPacket();
	l_wpk.WriteCmd(CMD_PC_GM1SAY);
	if( ply != NULL)
		l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());	//说话人的名字
	else
		{
			l_wpk.WriteString( Nsrc );
		}
	l_wpk.WriteString(l_content);								//说话的内容

	Player *l_plylst[10240];
	short	l_plynum	=0;

	Player	*	l_ply1	=0;char	l_currcha	=0;
	RunChainGetArmor<Player> l(m_plylst);
	while(l_ply1	=m_plylst.GetNextItem())
	{
		if((l_currcha =l_ply1->m_currcha) >=0)
		{
			l_plylst[l_plynum]	=l_ply1;
			l_plynum ++;
		}
	}
	Nsrc = NULL;
	l.unlock();
	SendToClient(l_plylst,l_plynum,l_wpk);
}

//Add by sunny.sun20080804
//Begin
void	GroupServerApp::CP_GM1SAY1(Player *ply,DataSocket *datasock,RPacket &pk)
{
	uShort		l_len;
	cChar	*	l_content	=pk.ReadString(&l_len);
	int			SetNum = 1;
	if(!l_content ||l_len >500)return;
	if(ply != NULL)
	{
		if(!ply->m_gm)
		{
			ply->SendSysInfo("你没有该权限.");
			return;
		}
	}
	else
		SetNum = pk.ReadLong();

	WPacket	l_wpk	=GetWPacket();
	l_wpk.WriteCmd(CMD_PC_GM1SAY1);
	//l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());	//说话人的名字
	l_wpk.WriteString(l_content);								//说话的内容
	l_wpk.WriteLong(SetNum);

	Player *l_plylst[10240];
	short	l_plynum	=0;

	Player	*	l_ply1	=0;char	l_currcha	=0;
	RunChainGetArmor<Player> l(m_plylst);
	while(l_ply1	=m_plylst.GetNextItem())
	{
		if((l_currcha =l_ply1->m_currcha) >=0)
		{
			l_plylst[l_plynum]	=l_ply1;
			l_plynum ++;
		}
	}
	l.unlock();
	SendToClient(l_plylst,l_plynum,l_wpk);
}
//End
void	GroupServerApp::CP_SAY2TRADE(Player *ply,DataSocket *datasock,RPacket &pk)
{
	if( ply->IsEstop() )
	{
		//ply->SendSysInfo( "你已经被禁言，请稍后再试！" );
		ply->SendSysInfo( RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00002) );
		return;
	}

	uLong	l_curtick	=this->GetCurrentTick();
	if(!ply->m_gm && l_curtick - ply->m_tradetick <const_interval.Trade)
	{
		//ply->SendSysInfo("请勿频繁发言");
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00003));
		return;
	}
	if(l_curtick - ply->m_tradetick < 3 * 60 * 1000)
	{
		if(ply->m_lTradeChatMoney < 100000)
		{
			ply->m_lTradeChatMoney += atoi(m_cfg["chat"]["WorldFeeInterval"]);//1000;
		}
	}
	else
	{
		ply->m_lTradeChatMoney = atoi(m_cfg["chat"]["WorldFee"]);//1000;
	}
	ply->m_tradeticktemp = ply->m_tradetick;
	ply->m_tradetick	=l_curtick;

	uShort		l_len;
	cChar	*	l_content	=pk.ReadString(&l_len);
	if(!l_content ||l_len >200)return;
	sendChatLog(ply, this, l_content, "Trade");
	if(ply->m_gm)
	{
		WPacket	l_wpk	=GetWPacket();
		l_wpk.WriteCmd(CMD_PC_SAY2TRADE);
		l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());	//说话人的名字
		l_wpk.WriteString(l_content);								//说话的内容
		l_wpk.WriteLong(ply->m_chatColour[ply->m_currcha]);
		Player *l_plylst[10240];
		short	l_plynum	=0;

		Player	*	l_ply1	=0;char	l_currcha	=0;
		RunChainGetArmor<Player> l(m_plylst);
		while(l_ply1	=m_plylst.GetNextItem())
		{
			if((l_currcha =l_ply1->m_currcha) >=0)
			{
				l_plylst[l_plynum]	=l_ply1;
				l_plynum ++;
			}
		}
		l.unlock();
		SendToClient(l_plylst,l_plynum,l_wpk);
		return;
	}

	WPacket	l_wpk = GetWPacket();
	l_wpk.WriteCmd(CMD_PM_SAY2TRADE);
	l_wpk.WriteLong(ply->m_chaid[ply->m_currcha]);
	l_wpk.WriteString( l_content );
	l_wpk.WriteLong(ply->m_lTradeChatMoney);
	SendToClient( ply, l_wpk );
}
void GroupServerApp::CP_SAY2ALL(Player *ply,DataSocket *datasock,RPacket &pk)
{
	if( ply->IsEstop() )
	{
		//ply->SendSysInfo( "你已经被禁言，请稍后再试！" );
		ply->SendSysInfo( RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00002) );
		return;
	}

	uLong	l_curtick	=this->GetCurrentTick();
	if(!ply->m_gm && l_curtick - ply->m_worldtick <const_interval.World)
	{
		//ply->SendSysInfo("请勿频繁发言");
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00003));
		return;
	}
	if(l_curtick - ply->m_worldtick < 3 * 60 * 1000)
	{
		if(ply->m_lChatMoney < 100000)
		{
			ply->m_lChatMoney += 1000;
		}
	}
	else
	{
		ply->m_lChatMoney = 1000;
	}
	ply->m_worldticktemp = ply->m_worldtick;
	ply->m_worldtick	=l_curtick;

	uShort		l_len;
	cChar	*	l_content	=pk.ReadString(&l_len);
	if(!l_content ||l_len >200)return;
	sendChatLog(ply, this, l_content, "World");
	if(ply->m_gm)
	{
		WPacket	l_wpk	=GetWPacket();
		l_wpk.WriteCmd(CMD_PC_SAY2ALL);
		l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());	//说话人的名字
		l_wpk.WriteString(l_content);								//说话的内容
		l_wpk.WriteLong(ply->m_chatColour[ply->m_currcha]);
		Player *l_plylst[10240];
		short	l_plynum	=0;

		Player	*	l_ply1	=0;char	l_currcha	=0;
		RunChainGetArmor<Player> l(m_plylst);
		while(l_ply1	=m_plylst.GetNextItem())
		{
			if((l_currcha =l_ply1->m_currcha) >=0)
			{
				l_plylst[l_plynum]	=l_ply1;
				l_plynum ++;
			}
		}
		l.unlock();
		SendToClient(l_plylst,l_plynum,l_wpk);
		return;
	}

	WPacket	l_wpk = GetWPacket();
	l_wpk.WriteCmd(CMD_PM_SAY2ALL);
	l_wpk.WriteLong(ply->m_chaid[ply->m_currcha]);
	l_wpk.WriteString( l_content );
	l_wpk.WriteLong(ply->m_lChatMoney);
	SendToClient( ply, l_wpk );
}

void GroupServerApp::CP_SAY2DIS(Player *ply, DataSocket *datasock, RPacket &pk){
	if (ply->IsEstop())
	{
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00002));
		return;
	}

	uShort		l_len;
	cChar	*	l_content = pk.ReadString(&l_len);
	if (!l_content || l_len >200)return;
	sendChatLog(ply, this, l_content, "Discord");
	WPacket	l_wpk = GetWPacket();
	l_wpk.WriteCmd(CMD_PC_SAY2DIS);
	l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());
	l_wpk.WriteString(l_content);
	l_wpk.WriteLong(ply->m_chatColour[ply->m_currcha]);
	Player *l_plylst[10240];
	short	l_plynum = 0;

	Player	*	l_ply1 = 0;
	char	l_currcha = 0;
	RunChainGetArmor<Player> l(m_plylst);
	while (l_ply1 = m_plylst.GetNextItem())
	{
		if ((l_currcha = l_ply1->m_currcha) >= 0 )
		{
			l_plylst[l_plynum] = l_ply1;
			l_plynum++;
		}
	}
	l.unlock();
	SendToClient(l_plylst, l_plynum, l_wpk);
}

void GroupServerApp::CP_SAY2VIP(Player *ply, DataSocket *datasock, RPacket &pk){
	if (ply->IsEstop())
	{
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00002));
		return;
	}

	if (!ply->m_gm)
	{
		ply->SendSysInfo("Only VIP may speak in this channel.");
		return;
	}

	uShort		l_len;
	cChar	*	l_content = pk.ReadString(&l_len);
	if (!l_content || l_len >200)return;
	sendChatLog(ply, this, l_content, "VIP");
	WPacket	l_wpk = GetWPacket();
	l_wpk.WriteCmd(CMD_PC_SAY2VIP);
	l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());	
	l_wpk.WriteString(l_content);								
	l_wpk.WriteLong(ply->m_chatColour[ply->m_currcha]);
	Player *l_plylst[10240];
	short	l_plynum = 0;

	Player	*	l_ply1 = 0;
	char	l_currcha = 0;
	RunChainGetArmor<Player> l(m_plylst);
	while (l_ply1 = m_plylst.GetNextItem())
	{
		if ((l_currcha = l_ply1->m_currcha) >= 0 && l_ply1->m_gm)
		{
			l_plylst[l_plynum] = l_ply1;
			l_plynum++;
		}
	}
	l.unlock();
	SendToClient(l_plylst, l_plynum, l_wpk);
}

void GroupServerApp::MP_SAY2TRADE(Player *ply,DataSocket *datasock,RPacket &pk)
{
	char succ = pk.ReadChar();
	if(succ == 0)
	{
		ply->m_tradetick = ply->m_tradeticktemp;
		if(ply->m_lChatMoney > 1000)
		{
			ply->m_lChatMoney -= 1000;
		}
		return;
	}

	cChar	*	szName		=pk.ReadString();
	cChar	*	l_content	=pk.ReadString();

	WPacket	l_wpk	=GetWPacket();
	l_wpk.WriteCmd(CMD_PC_SAY2TRADE);
	l_wpk.WriteString(szName);	//说话人的名字
	l_wpk.WriteString(l_content);								//说话的内容
	l_wpk.WriteLong(ply->m_chatColour[ply->m_currcha]);
	Player *l_plylst[10240];
	short	l_plynum	=0;

	Player	*	l_ply1	=0;char	l_currcha	=0;
	RunChainGetArmor<Player> l(m_plylst);
	while(l_ply1	=m_plylst.GetNextItem())
	{
		if((l_currcha =l_ply1->m_currcha) >=0)
		{
			l_plylst[l_plynum]	=l_ply1;
			l_plynum ++;
		}
	}
	l.unlock();
	SendToClient(l_plylst,l_plynum,l_wpk);
}

void GroupServerApp::MP_SAY2ALL(Player *ply,DataSocket *datasock,RPacket &pk)
{
	char succ = pk.ReadChar();
	if(succ == 0)
	{
		ply->m_worldtick = ply->m_worldticktemp;
		if(ply->m_lChatMoney > 1000)
		{
			ply->m_lChatMoney -= 1000;
		}
		return;
	}

	cChar	*	szName		=pk.ReadString();
	cChar	*	l_content	=pk.ReadString();

	WPacket	l_wpk	=GetWPacket();
	l_wpk.WriteCmd(CMD_PC_SAY2ALL);
	l_wpk.WriteString(szName);	//说话人的名字
	l_wpk.WriteString(l_content);								//说话的内容
	l_wpk.WriteLong(ply->m_chatColour[ply->m_currcha]);
	Player *l_plylst[10240];
	short	l_plynum	=0;

	Player	*	l_ply1	=0;char	l_currcha	=0;
	RunChainGetArmor<Player> l(m_plylst);
	while(l_ply1	=m_plylst.GetNextItem())
	{
		if((l_currcha =l_ply1->m_currcha) >=0)
		{
			l_plylst[l_plynum]	=l_ply1;
			l_plynum ++;
		}
	}
	l.unlock();
	SendToClient(l_plylst,l_plynum,l_wpk);
}

void GroupServerApp::MP_GUILDNOTICE(Player *ply, DataSocket *datasock, RPacket &pk)
{
	uLong guildID = pk.ReadLong();
	if (!guildID)
		return;

	cChar* l_content = pk.ReadString();

	WPacket	l_wpk = GetWPacket();
	l_wpk.WriteCmd(CMD_PC_GUILDNOTICE); // CMD_PC_GUILDNOTICE
	l_wpk.WriteString(l_content);

	Player *l_plylst[10240];
	short	l_plynum = 0;

	Player* l_ply1 = 0;
	char l_currcha = 0;

	RunChainGetArmor<Player> l(m_plylst);
	while (l_ply1 = (Player*)m_plylst.GetNextItem())
	{
		l_currcha = l_ply1->m_currcha;
		if (l_currcha >= 0)
		{
			Guild* chaGuild = l_ply1->GetGuild();
			if (!chaGuild)
				continue;

			uLong cha_guildID = chaGuild->m_id;
			if (cha_guildID == guildID)
			{
				l_plylst[l_plynum] = l_ply1;
				l_plynum++;
				continue;
			}
		}
	}
	l.unlock();
	SendToClient(l_plylst, l_plynum, l_wpk);
}

void GroupServerApp::CP_SAY2YOU(Player *ply, DataSocket *datasock, RPacket &pk)
{
	if (ply->IsEstop())
	{
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00002));
		return;
	}

	uLong	l_curtick = this->GetCurrentTick();
	if (!ply->m_gm && l_curtick - ply->m_toyoutick <const_interval.ToYou)
	{
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00003));
		return;
	}
	ply->m_toyoutick = l_curtick;

	uShort	l_len;
	cChar	*l_you = pk.ReadString(&l_len);
	if (!l_you || l_len >16)return;
	cChar	*l_content = pk.ReadString(&l_len);
	if (!l_content || l_len >200)return;

	if (l_you[0] == '@' && l_you[1] == 0)
	{
		ply->DoCommand(l_content);
	}
	else
	{
		Player	*l_dst = FindPlayerByChaName(l_you);
		if (!l_dst || l_dst->m_currcha <0)
		{

			if (l_you[strlen(l_you) - 5] == '#'){
				WPacket discord_wpk = GetWPacket();
				discord_wpk.WriteCmd(CMD_PM_PM2DISCORD);
				discord_wpk.WriteString(l_you);
				discord_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());
				discord_wpk.WriteString(l_content);
				SendToClient(ply, discord_wpk);

				//send it to player.
				WPacket l_wpk = GetWPacket();
				l_wpk.WriteCmd(CMD_PC_SAY2YOU);
				l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());
				l_wpk.WriteString(l_you);
				l_wpk.WriteString(l_content);
				l_wpk.WriteLong(ply->m_chatColour[ply->m_currcha]);
				SendToClient(ply, l_wpk);

				char PM[32];
				sprintf(PM, "PM - %s", l_you);
				sendChatLog(ply, this, l_content, PM);
			}
			else{
				char l_content[500];
				sprintf(l_content, RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00004), l_you);
				ply->SendSysInfo(l_content);
			}
		}
		else if (ply == l_dst)
		{
			ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00005));
		}
		else if (l_dst->m_refuse_tome)
		{
			char l_content[500];
			sprintf(l_content, RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00006), l_you);
			ply->SendSysInfo(l_content);
		}
		else
		{
			WPacket l_wpk = GetWPacket();
			l_wpk.WriteCmd(CMD_PC_SAY2YOU);
			l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());	
			l_wpk.WriteString(l_dst->m_chaname[l_dst->m_currcha].c_str());
			l_wpk.WriteString(l_content);		
			l_wpk.WriteLong(ply->m_chatColour[ply->m_currcha]);
			Player *l_dst1[2];
			l_dst1[0] = ply;
			l_dst1[1] = l_dst;
			SendToClient(l_dst1, 2, l_wpk);

			char PM[32];
			sprintf(PM, "PM - %s", l_you);
			sendChatLog(ply, this, l_content, PM);
		}
	}
}

void	GroupServerApp::CP_SAY2TEM(Player *ply,DataSocket *datasock,RPacket &pk)
{
	if( ply->IsEstop() )
	{
		//ply->SendSysInfo( "你已经被禁言，请稍后再试！" );
		ply->SendSysInfo( RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00002) );
		return;
	}

	uShort	l_len;
	Team	*l_team		=ply->GetTeam();
	if(!l_team)return;
	cChar	*l_word		=pk.ReadString(&l_len);
	if(!l_word ||l_len >200)return;
	sendChatLog(ply, this, l_word, "Party");

	Player *l_ply1;
	Player *l_plylst[10240];
	short	l_plynum	=0;

	RunChainGetArmor<TeamMember> l(*l_team);
	while(l_ply1 =static_cast<Player*>(l_team->GetNextItem()))
	{
		l_plylst[l_plynum]	=l_ply1;
		l_plynum ++;
	}
	l.unlock();

	WPacket l_wpk	=GetWPacket();
	l_wpk.WriteCmd(CMD_PC_SAY2TEM);
	l_wpk.WriteLong(ply->m_chaid[ply->m_currcha]);
	l_wpk.WriteString(l_word);
	l_wpk.WriteLong(ply->m_chatColour[ply->m_currcha]);
	SendToClient(l_plylst,l_plynum,l_wpk);
}
void	GroupServerApp::CP_SAY2GUD(Player *ply,DataSocket *datasock,RPacket &pk)
{
	if (ply->IsEstop())
	{
		//ply->SendSysInfo( "你已经被禁言，请稍后再试！" );
		ply->SendSysInfo( RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00002) );
		return;
	}

	int canSpeak = (ply->m_guildPermission[ply->m_currcha] & emGldPermSpeak);

	if (canSpeak != emGldPermSpeak){
		//ply->SendSysInfo( "你已经被禁言，请稍后再试！" );
		ply->SendSysInfo("You do not have permission to speak in guild.");
		return;
	}

	uShort	l_len;
	Guild	*l_guild = ply->GetGuild();
	if(!l_guild)return;
	cChar	*l_word		=pk.ReadString(&l_len);
	if(!l_word ||l_len >200)return;

	Player *l_ply1;
	Player *l_plylst[10240];
	short	l_plynum	=0;

	RunChainGetArmor<GuildMember> l(*l_guild);
	while(l_ply1 =static_cast<Player*>(l_guild->GetNextItem()))
	{
		l_plylst[l_plynum]	=l_ply1;
		l_plynum ++;
	}
	l.unlock();


	WPacket l_wpk	=GetWPacket();
	l_wpk.WriteCmd(CMD_PC_SAY2GUD);
	l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());		//说话人
	l_wpk.WriteString(l_word);
	l_wpk.WriteLong(ply->m_chatColour[ply->m_currcha]);
	SendToClient(l_plylst,l_plynum,l_wpk);

	char guild[32];
	Guild *l_gld = FindGuildByGldID(ply->m_guild[ply->m_currcha]);
	sprintf(guild, "Guild - %s", l_gld->m_name);
	sendChatLog(ply, this, l_word, guild);
}

//======================================================================

inline void sendfailue(Player *ply,GroupServerApp *gsapp,const char *chaname)
{
	if(!chaname)return;
	WPacket l_wpk =gsapp->GetWPacket();
	l_wpk.WriteCmd(CMD_PC_SESS_CREATE);
	l_wpk.WriteLong(0);
	l_wpk.WriteString(chaname);
	gsapp->SendToClient(ply,l_wpk);
}
void	GroupServerApp::CP_SESS_CREATE(Player *ply,DataSocket *datasock,RPacket &pk)
{
	uChar	l_chanum	=pk.ReadChar();
	if(!l_chanum)
	{
		return;
	}
	if(++(ply->m_chatnum) >const_chat.MaxSession)
	{
		--(ply->m_chatnum);
		if(l_chanum ==1){sendfailue(ply,this,pk.ReadString());}
		//ply->SendSysInfo("你当前的聊天窗口数已经达到系统的最大上限了.");
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00007));
		return;
	}
	if(l_chanum >const_chat.MaxPlayer -1)
	{
		--(ply->m_chatnum);
		if(l_chanum ==1){sendfailue(ply,this,pk.ReadString());}
		//ply->SendSysInfo("您要创建会话的玩家数过多.");
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00008));
		return;
	}
	const char		*	l_chaname;
	Chat_Session	*	l_sess			=0;
	int					l_notiplycount	=0;
	Player			*	l_notiply[Player::emMaxSessPlayer];
	Player			*	l_sessply;
	WPacket				l_wpk;
	uShort				l_len;
	for(uChar i=0;i<l_chanum;i++)
	{
		l_chaname	=pk.ReadString(&l_len);
		if(!l_chaname ||l_len >16)
		{
			continue;
		}
		if(!(l_sessply =FindPlayerByChaName(l_chaname)))
		{
			//ply->SendSysInfo(dstring("玩家【")<<l_chaname<<"】目前不在线上.");
			char l_content[500];
			sprintf(l_content,RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00009),l_chaname);
			ply->SendSysInfo(l_content);
			continue;
		}

		if(l_sessply ==ply)continue;
		if(l_sess)
		{
			Player	*l_ply1 =0;
			RunChainGetArmor<Chat_Player> l(*l_sess);
			while((l_ply1 =l_sess->GetNextPlayer()) &&l_ply1 !=l_sessply){}
			l.unlock();
			if(l_ply1)continue;
		}

		if(l_sessply->m_refuse_sess)
		{
			//ply->SendSysInfo(dstring("玩家【")<<l_chaname<<"】当前处于拒绝会话状态.");
			char l_content[500];
			sprintf(l_content,RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00010),l_chaname);
			ply->SendSysInfo(l_content);
			continue;
		}
		if(++(l_sessply->m_chatnum) >const_chat.MaxSession)
		{
			--(l_sessply->m_chatnum);
			//ply->SendSysInfo(dstring("玩家【")<<l_chaname<<"】的聊天窗口数已经达到系统的最大上限了.");
			char l_content[500];
			sprintf(l_content,RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00011),l_chaname);
			ply->SendSysInfo(l_content);
			continue;
		}
		if(!l_sess)
		{
			l_sess	=AddSession();
			l_sess->AddPlayer(ply);

			l_wpk	=GetWPacket();
			l_wpk.WriteCmd(CMD_PC_SESS_CREATE);
			l_wpk.WriteLong(l_sess->GetID());
			l_wpk.WriteLong(ply->m_chaid[ply->m_currcha]);
			l_wpk.WriteString(ply->m_chaname[ply->m_currcha].c_str());
			l_wpk.WriteString(ply->m_motto[ply->m_currcha].c_str());
			l_wpk.WriteShort(ply->m_icon[ply->m_currcha]);

			l_notiply[l_notiplycount]	=ply;
			++l_notiplycount;
		}
		l_sess->AddPlayer(l_sessply);

		l_wpk.WriteLong(l_sessply->m_chaid[l_sessply->m_currcha]);
		l_wpk.WriteString(l_sessply->m_chaname[l_sessply->m_currcha].c_str());
		l_wpk.WriteString(l_sessply->m_motto[l_sessply->m_currcha].c_str());
		l_wpk.WriteShort(l_sessply->m_icon[l_sessply->m_currcha]);

		l_notiply[l_notiplycount]	=l_sessply;
		++l_notiplycount;
	}
	if(l_notiplycount)
	{
		bool l_isRepeat	=false;
		MutexArmor lock(ply->m_mtxChat);
		for(uLong i=0;i<ply->m_chatarranum;i++)
		{
			Chat_Session	*l_sess1	=ply->m_chat[i]->GetSession();
			if(l_sess1 ==l_sess)continue;
			if(l_sess1->GetTotal() !=l_sess->GetTotal())continue;
			Player	*l_ply1	=0,*l_ply2	=0;
			RunChainGetArmor<Chat_Player> l(*l_sess1);
			while(l_ply1 =l_sess1->GetNextPlayer())
			{
				RunChainGetArmor<Chat_Player> l(*l_sess);
				while(l_ply2 =l_sess->GetNextPlayer())
				{
					if(l_ply1 ==l_ply2)break;
				}
				l.unlock();
				if(!l_ply2)break;
			}
			l.unlock();
			if(!l_ply1)
			{
				l_isRepeat	=true;
				break;
			}
		}
		lock.unlock();
		if(!l_isRepeat)
		{
			l_wpk.WriteShort(l_notiplycount);
			SendToClient(l_notiply,l_notiplycount,l_wpk);
		}else
		{
			Player *l_ply3;
			while(l_ply3 =l_sess->GetFirstPlayer())
			{
				l_sess->DelPlayer(l_ply3);
				--(l_ply3->m_chatnum);
			}
			if(l_chanum ==1){sendfailue(ply,this,l_chaname);}
			//ply->SendSysInfo("已经有一个相同成员的会话了.");
			ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00012));

			l_sess->DelPlayer(ply);
			--(ply->m_chatnum);
			DelSession(l_sess);
		}
	}else
	{
		if(l_chanum ==1){sendfailue(ply,this,l_chaname);}
		--(ply->m_chatnum);
	}
}
void	GroupServerApp::CP_SESS_ADD(Player *ply,DataSocket *datasock,RPacket &pk)
{
	uShort l_len;
	uLong	l_sessid		=pk.ReadLong();
	if(!l_sessid)return;
	cChar *	l_chaname		=pk.ReadString(&l_len);
	if(!l_chaname ||l_len >16)return;
	MutexArmor lock(ply->m_mtxChat);
	Chat_Session  *	l_sess	=ply->FindSessByID(l_sessid);
	if(!l_sess)return;

	Player	*l_sessply		=FindPlayerByChaName(l_chaname);
	if(!l_sessply || l_sessply->m_currcha <0)
	{
		//ply->SendSysInfo(dstring("玩家【")<<l_chaname<<"】目前不在线上。");************************
		char l_content[500];
		sprintf(l_content,RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00009),l_chaname);
		ply->SendSysInfo(l_content);
		return;
	}

	if(l_sessply ==ply)return;
	Player	*l_ply0 =0;
	RunChainGetArmor<Chat_Player> l(*l_sess);
	while((l_ply0 =l_sess->GetNextPlayer()) &&l_ply0 !=l_sessply){}
	l.unlock();
	if(l_ply0)
	{
		//ply->SendSysInfo(dstring("玩家【")<<l_chaname<<"】已经在你的聊天窗口里面.");
		char l_content[500];
		sprintf(l_content,RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00013),l_chaname);
		ply->SendSysInfo(l_content);
		return;
	}
	if(l_sessply->m_refuse_sess)
	{
		//ply->SendSysInfo(dstring("玩家【")<<l_chaname<<"】当前处于拒绝会话状态.");
		char l_content[500];
		sprintf(l_content,RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00010),l_chaname);
		ply->SendSysInfo(l_content);
		return;
	}

	if(++(l_sessply->m_chatnum) >const_chat.MaxSession)
	{
		--(l_sessply->m_chatnum);
		//ply->SendSysInfo(dstring("玩家【")<<l_chaname<<"】的聊天窗口数已经达到系统的最大上限了。");
		char l_content[500];
		sprintf(l_content,RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00011),l_chaname);
		ply->SendSysInfo(l_content);
		return;
	}
	if(l_sess->AddPlayer(l_sessply) >const_chat.MaxPlayer)
	{
		l_sess->DelPlayer(l_sessply);
		--(l_sessply->m_chatnum);
		//ply->SendSysInfo("会话的玩家数已经达到系统上限了，不能再添加了。");
		ply->SendSysInfo(RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00014));
		return;
	}
	int		 l_notiplycount	=0;
	Player	*l_notiply[Player::emMaxSessPlayer];
	Player	*l_sessply1;

	WPacket l_toAdded =GetWPacket();
	l_toAdded.WriteCmd(CMD_PC_SESS_CREATE);
	l_toAdded.WriteLong(l_sess->GetID());
	RunChainGetArmor<Chat_Player> ll(*l_sess);
	while(l_sessply1 =l_sess->GetNextPlayer())
	{
		if(l_sessply1->m_currcha >=0)
		{
			l_toAdded.WriteLong(l_sessply1->m_chaid[l_sessply1->m_currcha]);
			l_toAdded.WriteString(l_sessply1->m_chaname[l_sessply1->m_currcha].c_str());
			l_toAdded.WriteString(l_sessply1->m_motto[l_sessply1->m_currcha].c_str());
			l_toAdded.WriteShort(l_sessply1->m_icon[l_sessply1->m_currcha]);

			l_notiply[l_notiplycount] =l_sessply1;
			l_notiplycount ++;
		}
	}
	ll.unlock();
	if(l_notiplycount)
	{
		l_toAdded.WriteShort(l_notiplycount);
		SendToClient(l_sessply,l_toAdded);

		WPacket l_wpk =GetWPacket();
		l_wpk.WriteCmd(CMD_PC_SESS_ADD);
		l_wpk.WriteLong(l_sess->GetID());
		l_wpk.WriteLong(l_sessply->m_chaid[l_sessply->m_currcha]);
		l_wpk.WriteString(l_sessply->m_chaname[l_sessply->m_currcha].c_str());
		l_wpk.WriteString(l_sessply->m_motto[l_sessply->m_currcha].c_str());
		l_wpk.WriteShort(l_sessply->m_icon[l_sessply->m_currcha]);
		SendToClient(l_notiply,l_notiplycount,l_wpk);
	}else
	{
		--(l_sessply->m_chatnum);
	}
}
void	GroupServerApp::CP_SESS_LEAVE(Player *ply,DataSocket *datasock,RPacket &pk)
{
	uLong	l_sessid		=pk.ReadLong();
	if(!l_sessid)return;
	MutexArmor lock(ply->m_mtxChat);
	Chat_Session  *	l_sess	=ply->FindSessByID(l_sessid);
	if(!l_sess)return;
	long l_num	=l_sess->DelPlayer(ply);
	if(!l_num)return;
	--(ply->m_chatnum);

	int		 l_notiplycount	=0;
	Player	*l_notiply[Player::emMaxSessPlayer];
	Player	*l_sessply;

	WPacket l_wpk	=GetWPacket();
	l_wpk.WriteCmd(CMD_PC_SESS_LEAVE);
	l_wpk.WriteLong(l_sessid);
	l_wpk.WriteLong(ply->m_chaid[ply->m_currcha]);
	if(l_num >1)
	{
		RunChainGetArmor<Chat_Player> l(*l_sess);
		while(l_sessply =l_sess->GetNextPlayer())
		{
			l_notiply[l_notiplycount] =l_sessply;
			++l_notiplycount;
		}
		l.unlock();
	}
	l_notiply[l_notiplycount] =ply;
	++l_notiplycount;

	SendToClient(l_notiply,l_notiplycount,l_wpk);
	if(l_num == 2)
	{
		WPacket l_wpk	=GetWPacket();
		l_wpk.WriteLong(l_sessid);
		CP_SESS_LEAVE(l_sess->GetFirstPlayer(),datasock,RPacket(l_wpk));
	}else if(l_num == 1)
	{
		DelSession(l_sess);
	}
}
void	GroupServerApp::CP_SESS_SAY(Player *ply,DataSocket *datasock,RPacket &pk)
{
	if( ply->IsEstop() )
	{
		//ply->SendSysInfo( "你已经被禁言，请稍后再试！" );
		ply->SendSysInfo( RES_STRING(GP_GROUPSERVERAPPCHAT_CPP_00002) );
		return;
	}

	uShort	l_len;
	uLong	l_sessid		=pk.ReadLong();
	if(!l_sessid)return;
	cChar *	l_word			=pk.ReadString(&l_len);
	if(!l_word ||l_len >200)return;
	Chat_Session  *	l_sess	=ply->FindSessByID(l_sessid);
	if(!l_sess)return;

	int		 l_notiplycount	=0;
	Player	*l_notiply[Player::emMaxSessPlayer];
	Player	*l_sessply;

	WPacket	l_wpk	=GetWPacket();
	l_wpk.WriteCmd(CMD_PC_SESS_SAY);
	l_wpk.WriteLong(l_sess->GetID());
	l_wpk.WriteLong(ply->m_chaid[ply->m_currcha]);	//说话人
	l_wpk.WriteString(l_word);
	RunChainGetArmor<Chat_Player> l(*l_sess);
	while(l_sessply =l_sess->GetNextPlayer())
	{
		l_notiply[l_notiplycount]	=l_sessply;
		++l_notiplycount;
	}
	l.unlock();
	if(l_notiplycount)
	{
		SendToClient(l_notiply,l_notiplycount,l_wpk);
		sendChatLog(ply, this, l_word, "ChatBox");
	}
}
