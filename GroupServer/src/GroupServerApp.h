/* Generated by Together */

#ifndef GRPNAMESVR_H
#define GRPNAMESVR_H
#include "ThreadPool.h"
#include "CommRPC.h"
#include "PreAlloc.h"
#include "RunCtrlThrd.h"
#include "IniFile.h"
#include "LogStream.h"

#include "GateServer.h"
#include "Player.h"
#include "Guild.h"
#include "Team.h"
#include "SessionChat.h"
#include "DBConnect.h"

#include "i18n.h"

// Add by lark.li 20080801 begin
#include "pi_Memory.h"
#include "pi_Alloc.h"
// End

_DBC_USING

class GroupServerApp : public TcpServerApp, public RPCMGR {
public:
	GroupServerApp(ThreadPool *proc,ThreadPool *comm);
	~GroupServerApp();

	uLong	m_dwCheatCount;	//使用外挂个数
public:
	friend void Player::EndPlay(DataSocket *datasock);
	void Initialize();
	void Finalize();
	virtual	bool	OnConnect(DataSocket *datasock);					//返回值:true-允许连接,false-不允许连接
	virtual void	OnDisconnect(DataSocket *datasock,int reason);
	virtual	void	OnProcessData(DataSocket *datasock,RPacket &recvbuf);
	virtual	WPacket	OnServeCall(DataSocket *datasock,RPacket &in_para);
	bool GetCHAsFromDBByPlayer(Player *player,WPacket &wpk);

	
	WPacket	TP_REGISTER(DataSocket *datasock, RPacket &pk);
	WPacket	TP_LOGIN(DataSocket *datasock,RPacket &pk);
	WPacket	TP_USER_LOGIN(DataSocket *datasock,RPacket &pk);
	WPacket TP_USER_LOGOUT(Player *ply,DataSocket *datasock,RPacket &pk);
	WPacket	TP_BGNPLAY(Player *ply,DataSocket *datasock,RPacket &pk);
	WPacket TP_ENDPLAY(Player *ply,DataSocket *datasock,RPacket &pk);
	WPacket	TP_NEWCHA(Player *ply,DataSocket *datasock,RPacket &pk);
	WPacket	TP_DELCHA(Player *ply,DataSocket *datasock,RPacket &pk);
	WPacket	TP_CREATE_PASSWORD2(Player *ply,DataSocket *datasock,RPacket &pk);
	WPacket TP_UPDATE_PASSWORD2(Player *ply,DataSocket *datasock,RPacket &pk);
	WPacket	TP_CHANGEPASS(Player * l_ply, DataSocket *datasock, RPacket &pk);


	void	MP_ENTERMAP(Player *ply,DataSocket *datasock,RPacket &pk);
	void	MP_ONLINE(Player *ply);
	void	MP_SWITCH(Player *ply);
	void	PC_FRND_INIT(Player *ply);
	void	PC_GM_INIT(Player *ply);
	void	PC_GULD_INIT(Player *ply);
	void	PC_MASTER_INIT(Player *ply);

	void	MP_TEAM_CREATE(Player *ply,DataSocket *datasock,RPacket &pk);

	void	MP_MASTER_CREATE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	MP_MASTER_DEL(Player *ply,DataSocket *datasock,RPacket &pk);
	void	MP_MASTER_FINISH(Player *ply,DataSocket *datasock,RPacket &pk);

	WPacket	TP_REQPLYLST(DataSocket *datasock,RPacket &pk);
	void	AP_KICKUSER(DataSocket *datasock,RPacket &pk);
	void	AP_KICKUSER2( DataSocket* datasock, uLong acctid );
    void    AP_EXPSCALE(DataSocket* datasock, RPacket &pk); //  防沉迷

	void	TP_DISC(DataSocket *datasock,RPacket &pk);
	void	TP_ESTOPUSER_CHECK(DataSocket *datasock,RPacket &pk);

	void	CP_TEAM_INVITE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_TEAM_ACCEPT(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_TEAM_REFUSE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_TEAM_LEAVE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_TEAM_KICK(Player *ply,DataSocket *datasock,RPacket &pk);

	void	CP_FRND_INVITE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_FRND_ACCEPT(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_FRND_REFUSE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_FRND_DELETE(Player *ply,DataSocket *datasock,RPacket &pk);

	void	CP_FRND_CHANGE_GROUP(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_FRND_REFRESH_INFO(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_CHANGE_PERSONINFO(Player *ply,DataSocket *datasock,RPacket &pk);

	void	CP_MASTER_INVITE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_MASTER_ACCEPT(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_MASTER_REFUSE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_MASTER_DELETE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_PRENTICE_DELETE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_MASTER_REFRESH_INFO(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_PRENTICE_REFRESH_INFO(Player *ply,DataSocket *datasock,RPacket &pk);

	void	CP_REFUSETOME(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_GM1SAY(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_GM1SAY1(Player *ply,DataSocket *datasock,RPacket &pk);//Add by sunny.sun20080804
		
	void	CP_SAY2VIP(Player *ply, DataSocket *datasock, RPacket &pk);
	void	CP_SAY2DIS(Player *ply, DataSocket *datasock, RPacket &pk);
	void	CP_SAY2TRADE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_SAY2ALL(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_SAY2YOU(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_SAY2TEM(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_SAY2GUD(Player *ply,DataSocket *datasock,RPacket &pk);

	void	MP_SAY2ALL(Player *ply,DataSocket *datasock,RPacket &pk);
	void	MP_SAY2TRADE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	MP_GUILDNOTICE(Player *ply, DataSocket *datasock, RPacket &pk);

	void	CP_SESS_CREATE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_SESS_SAY(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_SESS_ADD(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_SESS_LEAVE(Player *ply,DataSocket *datasock,RPacket &pk);

	void	MP_GUILD_APPROVE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	MP_GUILD_CREATE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	MP_GUILD_KICK(Player *ply,DataSocket *datasock,RPacket &pk);
	void	MP_GUILD_LEAVE(Player *ply,DataSocket *datasock,RPacket &pk);
	void	MP_GUILD_DISBAND(Player *ply,DataSocket *datasock,RPacket &pk);
	void	MP_GUILD_MOTTO(Player *ply,DataSocket *datasock,RPacket	&pk);
	void	MP_GUILD_CHALLMONEY(Player *ply,DataSocket *datasock,RPacket &pk); 
	void	MP_GUILD_CHALL_PRIZEMONEY(Player *ply,DataSocket *datasock,RPacket &pk); 

	void	MP_GM_BANACCOUNT(Player *ply, DataSocket *datasock,RPacket &pk); 
	void	MP_GM_UNBANACCOUNT(Player *ply, DataSocket *datasock,RPacket &pk); 

	void	CP_PING(Player *ply,DataSocket *datasock,RPacket &pk);
	void	CP_REPORT_WG(Player *ply,DataSocket *datasock,RPacket &pk);	

	// Add by lark.li 20081119 begin
	WPacket	TP_SYNC_PLYLST(DataSocket *datasock,RPacket &pk);
	WPacket	OS_LOGIN(DataSocket *datasock,RPacket &pk);
	void	OS_PING(DataSocket *datasock,RPacket &pk);
	// End

	void	MP_GARNER2_UPDATE(Player *ply,DataSocket *datasock,RPacket &pk);//反斗白银，更新排名。

	bool	CheckFunction(string mapName, string funName);
public:
	void	CP_GARNER2_GETORDER(Player *ply,DataSocket *datasock,RPacket &pk);//客户端请求反斗白银排名。
	void	KickUser(DataSocket *datasock,uLong gpaddr,uLong gtaddr);
	void	SendToClient(Player* ply[],short cli_num,const WPacket &wpk);
	void	SendToClient(Player* ply,WPacket &wpk);
	GateServer	*	FindGateSByName(cChar *gatename);
	Guild		* 	FindGuildByName(cChar * gldname);
	Guild		* 	FindGuildByGldID(uLong	gldid);
	Guild		*	FindGuildByLeadID(uLong chaid);
	Player		*	FindPlayerByChaName(cChar * plyname);
	Player		*	FindPlayerByChaID(uLong chaid);

	Player *GetPlayerByChaID(uLong chaid); //快速获取角色指针
	bool AddPlayerToList(uLong chaid, Player *pPlayer);
	bool DelPlayerFromList(uLong chaid);

	enum{GATE_MAX	=50};
	GateServer		m_gate[GATE_MAX];			//链表指针

	// Add by lark.li 20081119 begin
	GroupServerAgent m_groupServerAgent;
	// End

	struct
	{
		Mutex			m_mtxlogin;
		Mutex			m_mtxSyn;
		short			m_gatenum;
		DataSocket	*	m_acctsock;
		dstring			m_name;
		// Add by lark.li 20081119 begin
		Mutex			m_mtxAgent;
		// End
	};

	//内部数据结构：
	PreAllocHeap<Player>		m_plyheap;
	RunBiDirectChain<Player> 	m_plylst;
	PreAllocHeap<Guild>			m_gldheap;
	RunBiDirectChain<Guild> 	m_gldlst;
	PreAllocHeap<Team>			m_teamheap;
	RunBiDirectChain<Team>		m_teamlst;
	RunCtrlMgr<Invitation>		m_plyproc;

	RunBiDirectChain<Chat_Session>		m_sesslst;
	Chat_Session *AddSession();
	void  DelSession(Chat_Session * sess);

	//数据库连接：
	struct
	{
		Mutex			m_mtxDB;
		cfl_db			m_cfg_db;
		TBLAccounts	*	m_tblaccounts;
		TBLCharacters*	m_tblcharaters;
		TBLFriends	*	m_tblfriends;
		TBLMaster	*	m_tblmaster;
		TBLGuilds	*	m_tblguilds;
		TBLParam	*	m_tbLparam;
		friend_tbl	*	m_tblX1;
		IniFile			m_cfg;
		InterLockedLong	m_curChaNum;
		InterLockedLong	m_curWGChaNum;
	};
	struct
	{
		char			MaxChaNum;
		short			MaxIconVal;
		short			MaxLoginUsr;
	} const_cha;
	struct
	{
		char  InvitedMax;		// =5;
		uLong PendTimeOut;		//=30*1000
		char  FriendMax;		//=100
		char  FriendGroupMax;	//=10
	} const_frnd;
	struct
	{
		char  InvitedMax;		// =5
		uLong PendTimeOut;		//=30*1000
		char  MemberMax;		// =5
	} const_team;
	struct
	{
		char  InvitedMax;		// =5;
		uLong PendTimeOut;		//=30*1000
		char  MasterMax;		//=1
		char  PrenticeMax;		//=4
	} const_master;
	struct
	{
		uShort MaxSession;		// =3
		uShort MaxPlayer;		// =30
	} const_chat;
	struct
	{
		uLong  World;
		uLong  Trade;
		uLong  ToYou;
	} const_interval;
//	int nOrder[MAXORDERNUM];
private:
	std::vector<int> gmLogged;
	bool InitMasterRelation();
	int GetMasterCount(uLong cha_id);
	int GetPrenticeCount(uLong cha_id);
	int HasMaster(uLong cha_id1,uLong cha_id2);
	bool AddMaster(uLong cha_id1,uLong cha_id2);
	bool DelMaster(uLong cha_id1,uLong cha_id2);
	bool FinishMaster(uLong cha_id);

	map<uLong, uLong> m_mapMasterRelation; //师徒关系表
	map<uLong, Player *> m_mapPlayerList; //角色映射表

	// Add by lark.li 20080702 begin
	map<string, string> m_mapBirthplace; //出生地
	// End
};
class GMBBS:public Timer
{
	enum{em_words	=30};
public:
	GMBBS(uLong interval);
	bool AddBBS(uLong inter,uLong times,const char *word);
private:
	~GMBBS();
	void Process();
	struct
	{
		uLong	volatile	m_start;
		uLong	volatile	m_inter;
		uLong	volatile	m_times;
		dstring			 	m_word;
	}	m_queue[em_words];
	Mutex	m_mtxque;

	uLong	m_dwCount;
};

extern void InitACTSvrConnect(GroupServerApp &gpapp);	//AccountServer连接
extern GroupServerApp * g_gpsvr;
extern InterLockedLong	g_exit;
extern InterLockedLong	g_ref;
extern LogStream			g_LogGrpServer;
extern LogStream			g_LogErrServer;
extern LogStream			g_LogGuild;
extern LogStream			g_LogFriend;
extern LogStream			g_LogMaster;
extern LogStream			g_LogTeam;
extern LogStream			g_LogConnect;
extern LogStream			g_LogDB;
extern LogStream			g_LogGarner2;
extern GMBBS		*		g_gmbbs;
extern HANDLE				hConsole;

#define C_PRINT(s, ...) \
	SetConsoleTextAttribute(hConsole, 14); \
	printf(s, __VA_ARGS__); \
	SetConsoleTextAttribute(hConsole, 10);

#define C_TITLE(s) \
	char szPID[32]; \
	_snprintf_s(szPID,sizeof(szPID),_TRUNCATE, "%d", GetCurrentProcessId()); \
	std::string strConsoleT; \
	strConsoleT += "[PID:"; \
	strConsoleT += szPID; \
	strConsoleT += "]"; \
	strConsoleT += s; \
	SetConsoleTitle(strConsoleT.c_str());

#endif //GRPNAMESVR_H
