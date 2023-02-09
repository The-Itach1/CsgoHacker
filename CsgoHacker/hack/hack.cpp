#include "../includes.h"

std::unique_ptr<CHack>m_pHack = std::make_unique<CHack>();


bool CHack::GetHandle()
{
	DWORD dwPid;

	GetWindowThreadProcessId(globals::hGame, &dwPid);
	m_hprocess= OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

	m_hclient = GetModuleHandle("client.dll");
	m_hengine = GetModuleHandle("engine.dll");

	

	printf("%x %x %x %x\n", (DWORD)m_hengine, (DWORD)m_hclient,(DWORD)m_hclient+offsets::local_player , (DWORD)m_hclient + offsets::view_matrix);

	printf("%x ", (DWORD)m_hclient + offsets::entity_list + 1 * 0x10);

	if (m_hengine && m_hclient && m_hprocess)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//Matrix4x4* CHack::GetViewMatrix()
//{
//	this->view_matrix = (Matrix4x4*)memory::read<DWORD>(m_hprocess, (DWORD)m_hclient + offsets::view_matrix);
//
//	if (!IsValidPtr(this->view_matrix))
//		return NULL;
//	
//	return this->view_matrix;
//}

void CHack::Run()
{
	if (vars::global::enable == false)
		return;

	//更新本地玩家信息
	if (this->UpdateLocalPlayer() == false)
		return;

	//更新所有玩家
	this->UpdatePlayers();



	if (this->g_Local.m_flPlayerHealth > 0 && this->g_Local.m_flPlayerHealth<=100)
	{
		this->Aimbot();
		this->PlayerESP();
		this->GlowESP();
	}
	
	m_pHack->Debug();

	this->ClearPlayersData();
}


bool CHack::UpdateLocalPlayer()
{
	ClientPlayer* cClientPlayer=(ClientPlayer*) memory::read<DWORD>(m_hprocess, (DWORD)m_hclient + offsets::local_player);

	if (!IsValidPtr(cClientPlayer))
		return false; 

	if (cClientPlayer->IsDead())
		return false;

	if (!cClientPlayer->IsValid())
		return false;

	this->g_Local.m_iTeam = cClientPlayer->teamId;
	this->g_Local.m_flPlayerHealth = cClientPlayer->Health;
	this->g_Local.m_vOrigin = cClientPlayer->loction;
	this->g_Local.m_lifeState = cClientPlayer->lifestate;
	//this->g_Local.m_glowIndex = cClientPlayer->glowIndex;
	this->g_Local.m_vBoneOrigin[0] = cClientPlayer->GetHeadOrigin();
	this->g_Local.m_vBoneOrigin[1] = cClientPlayer->GetNeckOrigin();
	this->g_Local.m_vBoneOrigin[2] = cClientPlayer->GetChestOrigin();
	this->g_Local.m_vBoneOrigin[3] = cClientPlayer->GetStomachOrigin();
	this->g_Local.m_vBoneOrigin[4] = cClientPlayer->GetWaistOrigin();

	return true;
}


void CHack::UpdatePlayers()
{
	for (int i = 1; i <= 32; i++)
	{
		ClientPlayer* cPlayer = (ClientPlayer*)memory::read<DWORD>(m_hprocess, (DWORD)m_hclient + offsets::entity_list + i * 0x10);

		if (!IsValidPtr(cPlayer))
			continue;

		if (cPlayer->IsDead())
			continue;
		if (!cPlayer->IsValid())
			continue;
		
		extra_players_data_s player;
		player.m_Index = i;
		player.m_iTeam = cPlayer->teamId;
		player.m_flPlayerHealth = cPlayer->Health;
		player.m_vOrigin = cPlayer->loction;
		player.m_lifeState = cPlayer->lifestate;
		player.m_glowIndex = memory::read<DWORD>(m_hprocess, (DWORD)cPlayer + offsets::m_iGlowIndex);
		player.m_vBoneOrigin[0] = cPlayer->GetHeadOrigin();
		player.m_vBoneOrigin[1] = cPlayer->GetNeckOrigin();
		player.m_vBoneOrigin[2] = cPlayer->GetChestOrigin();
		player.m_vBoneOrigin[3] = cPlayer->GetStomachOrigin();
		player.m_vBoneOrigin[4] = cPlayer->GetWaistOrigin();

		this->g_Players.push_back(player);
	}
	

}

void CHack::ClearPlayersData()
{
	if (!this->g_Players.empty())
	{
		this->g_Players.clear();
		
	}
}

void CHack::Debug()
{
	if (vars::extra::debug == false)
		return;

	ImGui::Begin("Debug", nullptr);
	ImGui::Text("%08x", this->g_Local.m_lifeState);
	ImGui::Text("%d", this->g_Local.m_flPlayerHealth);
	ImGui::Text("%.0f", this->g_Local.m_vOrigin.x);
	ImGui::Text("%.0f", this->g_Local.m_vOrigin.y);
	ImGui::Text("%.0f", this->g_Local.m_vOrigin.z);


	ImGui::Text("%.0f", this->g_Local.m_vBoneOrigin[0].x);
	ImGui::Text("%.0f", this->g_Local.m_vBoneOrigin[0].y);
	ImGui::Text("%.0f", this->g_Local.m_vBoneOrigin[0].z);

	CViewMatrix* cViewMatrix = (CViewMatrix*)((DWORD)m_pHack->m_hclient + offsets::view_matrix);

	ImGui::Text("%x", cViewMatrix);
	ViewMatrix view_projection = cViewMatrix->view_matrix;
	ImGui::Text("m : %lf   %lf  %lf  %lf", view_projection.m[0][0], view_projection.m[0][1], view_projection.m[0][2], view_projection.m[0][3]);
	ImGui::Text("m : %lf   %lf  %lf  %lf", view_projection.m[1][0], view_projection.m[1][1], view_projection.m[1][2], view_projection.m[1][3]);
	ImGui::Text("m : %lf   %lf  %lf  %lf", view_projection.m[2][0], view_projection.m[2][1], view_projection.m[2][2], view_projection.m[2][3]);
	ImGui::Text("m : %lf   %lf  %lf  %lf", view_projection.m[3][0], view_projection.m[3][1], view_projection.m[3][2], view_projection.m[3][3]);
	
	ImGui::Text("num: %d", this->g_Players.size());

	/*
	if (!this->g_Players.empty())
	{
		for (unsigned int i = 0; i < this->g_Players.size(); i++)
		{
			ImGui::Text("%x", this->g_Players[i].m_glowIndex);
		}

		//ImGui::Text("%.0f", this->g_Players[i].m_vOrigin.x);
		//ImGui::Text("%.0f", this->g_Players[i].m_vOrigin.y);
		//ImGui::Text("%.0f", this->g_Players[i].m_vOrigin.z);

		//ImGui::Text("%.0f", this->g_Players[i].m_vBoneOrigin[0].x);
		//ImGui::Text("%.0f", this->g_Players[i].m_vBoneOrigin[0].y);
		//ImGui::Text("%.0f", this->g_Players[i].m_vBoneOrigin[0].z);
	}
	*/

	ImGui::End();
}


void CHack::ScreenInfo()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.1f);
	ImGui::Begin("##info", nullptr,
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 229.f, 8.f));
	ImGui::SetWindowSize(ImVec2(219.f, 10.f));
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%H:%M:%S", timeinfo);
	ImGui::Text("Csgo | ");
	ImGui::SameLine();
	ImGui::Text("%.f fps | ", ImGui::GetIO().Framerate);
	float calc_x = ImGui::CalcTextSize(buffer).x;
	ImGui::SameLine(ImGui::GetWindowWidth() - calc_x - 9.f);
	ImGui::Text(buffer);
	ImGui::End();
	ImGui::PopStyleVar();
}


void CHack::PlayerESP()
{
	LINE_STATUS_BAR StatusbarWay= LINE_STATUS_BAR::LEFT;
	
	switch (vars::visuals::statusbar_way)
	{
	case 0:
		StatusbarWay = LINE_STATUS_BAR::LEFT;
		break;
	case 1:
		StatusbarWay = LINE_STATUS_BAR::RIGHT;
		break;
	case 2:
		StatusbarWay = LINE_STATUS_BAR::UPPER;
		break;
	case 3:
		StatusbarWay = LINE_STATUS_BAR::BOTTOM;
		break;
	default:
		break;
	}

	for (auto p : this->g_Players)
	{
		if (vars::visuals::player_type == 0 && p.m_iTeam == this->g_Local.m_iTeam)
			continue;

		if (vars::visuals::player_type == 2)
			return;
		ImColor col_box = PlayerColor(p.m_iTeam);
		Vector vTop = p.m_vBoneOrigin[0];
		Vector vBot = p.m_vOrigin;

		float flTop[2];
		float flBot[2];

		if (WorldToScreen(vTop, flTop) && WorldToScreen(vBot, flBot))
		{
			float h = flBot[1] - flTop[1];
			float w = h / 2;
			float x = flBot[0] - w / 2;
			float y = flTop[1];

			this->DrawBox(x, y, w, h, col_box);

			this->DrawStatusLine(x, y, w, h, p.m_flPlayerHealth, 100, ImColor(0.f, 1.f, 0.f), StatusbarWay);

			this->DrawDistance(x, y, w, h, p.m_vOrigin.Distance(this->g_Local.m_vOrigin));
		}
	}
}



void CHack::DrawBox(float x, float y, float w, float h, const ImColor col)
{
	m_pDrawing->DrawEspBox(vars::visuals::box_type, x, y, w, h, col.Value.x, col.Value.y, col.Value.z, col.Value.w);
}

void CHack::DrawStatusLine(float x, float y, float w, float h, float status_value, float max_of_status_value, ImColor col, LINE_STATUS_BAR status_side)
{
	if (vars::visuals::health==false)
		return;
	if (status_value <= 0.f)
		return;

	status_value = ImClamp(status_value, 0.f, max_of_status_value);

	const auto size_h = h / max_of_status_value * status_value;
	const auto size_w = w / max_of_status_value * status_value;

	const auto thickness = 2.f;

	switch (status_side)
	{
	case LINE_STATUS_BAR::LEFT:
		m_pDrawing->DrawFillArea(x - thickness - 1.9f, y + h, thickness, -size_h, ImColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w));

		if (vars::visuals::box_type == 2 || vars::visuals::box_type == 4 || vars::visuals::box_type == 6)
			m_pDrawing->DrawBox(x - thickness - 2.9f, y - 1.f, thickness + 2.f, h + 2.f, ImColor(0.f, 0.f, 0.f, col.Value.w));

		break;
	case LINE_STATUS_BAR::RIGHT:
		m_pDrawing->DrawFillArea(x + w - thickness + (2.9f * 2.f), y + h, thickness, -size_h, ImColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w));

		if (vars::visuals::box_type == 2 || vars::visuals::box_type == 4 || vars::visuals::box_type == 6)
			m_pDrawing->DrawBox(x + w - thickness + (2.9f * 2.f) - 0.9f, y - 1.f, thickness + 2.f, h + 2.f, ImColor(0.f, 0.f, 0.f, col.Value.w));

		break;
	case LINE_STATUS_BAR::UPPER:
		m_pDrawing->DrawFillArea(x, y - thickness - 1.9f, size_w + 1.9f, thickness, ImColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w));

		if (vars::visuals::box_type == 2 || vars::visuals::box_type == 4 || vars::visuals::box_type == 6)
			m_pDrawing->DrawBox(x - 1.9f, y - thickness - 2.9f, w + 3.9f, thickness + 2.f, ImColor(0.f, 0.f, 0.f, col.Value.w));

		break;
	case LINE_STATUS_BAR::BOTTOM:
		m_pDrawing->DrawFillArea(x, y + h + thickness + 0.9f, size_w + 1.9f, thickness, ImColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w));

		if (vars::visuals::box_type == 2 || vars::visuals::box_type == 4 || vars::visuals::box_type == 6)
			m_pDrawing->DrawBox(x - 1.9f, y + h + thickness, w + 3.9f, thickness + 2.f, ImColor(0.f, 0.f, 0.f, col.Value.w));

		break;
	default: break;
	}
}

void CHack::DrawDistance(float x, float y, float w, float h, float distance)
{
	if (vars::visuals::distance == false)
		return;

	m_pDrawing->AddText(x + w / 2.f, y + h + 4.9f, ImColor(1.f, 1.f, 1.f), vars::font::size, FL_CENTER_X, "%.0f", distance);
}

void CHack::Draw3DCircle(Vector vOrigin, ImColor col)
{

	m_pDrawing->AddCircle3D(vOrigin, 18.f, 0.8f, col);
}


ImColor CHack::PlayerColor(int iTeam)
{
	if (iTeam == this->g_Local.m_iTeam)
	{
		return ImColor(vars::visuals::col_teammate[0], vars::visuals::col_teammate[1], vars::visuals::col_teammate[2], 1.f);
	}
	else if (iTeam != this->g_Local.m_iTeam)
	{
		return ImColor(vars::visuals::col_enemy[0], vars::visuals::col_enemy[1], vars::visuals::col_enemy[2], 1.f);
	}
	else
		return ImColor(1.f, 1.f, 1.f);
}


void CHack::Aimbot()
{
	if (globals::killed_focus)
		return;

	if (vars::aimbot::enable == false)
		return;

	if (vars::bMenuOpen)
		return;


	float flFov = vars::aimbot::fov * 10.f;

	int iBone = 0;

	switch (vars::aimbot::bone)
	{
	case 0:
		iBone = BONE::HEAD;
		break;
	case 1:
		iBone = BONE::NECK;
		break;
	case 2:
		iBone = BONE::CHEST;
		break;
	case 3:
		iBone = BONE::STOMACH;
		break;
	case 4:
		iBone = BONE::WAIST;
		break;
	default:
		break;
	}


	bool isPressedMLeftButton = GetAsyncKeyState(VK_LBUTTON);
	bool isPressedMRightButton = GetAsyncKeyState(VK_LSHIFT);

	bool event = vars::aimbot::aiming_on_rmb ?
		isPressedMLeftButton == false && isPressedMRightButton == false
		: isPressedMLeftButton == false;

	if (event)
		return;



	int iTarget = INT_MAX;
	float flMaxClosetsToScreenCenter = flFov;
	for (unsigned int i = 0; i < this->g_Players.size(); i++)
	{

		if (vars::aimbot::only_enemy && this->g_Players[i].m_iTeam == this->g_Local.m_iTeam)
			continue;

		Vector vTarget;

		vTarget = this->g_Players[i].m_vBoneOrigin[iBone];

		float flScreenTarget[2];

		if (WorldToScreen(vTarget, flScreenTarget))
		{
			//获取到屏幕中心的绝对值
			float flDifferenceFromScreenCenter[2] = {
				abs((ImGui::GetIO().DisplaySize.x / 2) - flScreenTarget[0]),
				abs((ImGui::GetIO().DisplaySize.y / 2) - flScreenTarget[1])
			};
			//确保敌人位置在设定的中心范围内，并且会不断修改flMaxClosetsToScreenCenter，也就是最终会返回离中心坐标最近的那个敌人的下标。
			if (flDifferenceFromScreenCenter[0] <= flFov && flDifferenceFromScreenCenter[1] <= flFov
				&& flDifferenceFromScreenCenter[0] < flMaxClosetsToScreenCenter)
			{
				flMaxClosetsToScreenCenter = flDifferenceFromScreenCenter[0];
				iTarget = i;
			}
		}

	}

	if (iTarget == INT_MAX)
		return;

	if (iBone == BONE::NONE)
		return;

	Vector vTarget;
	

	float AimbotAngle[3];
	vTarget = this->g_Players[iTarget].m_vBoneOrigin[iBone];

	CalcAngle(this->g_Local.m_vBoneOrigin[0], vTarget, AimbotAngle);

	AimbotAngle[0] += vars::aimbot::downoffset;
	AimbotAngle[1] += vars::aimbot::transverseoffset;
	DWORD CBaseXY;

	if (ReadProcessMemory(m_hprocess, (PBYTE*)((DWORD)m_hengine + offsets::dwClientState), &CBaseXY, sizeof(DWORD), 0) == false)
		return;

	if (!IsValidPtr(CBaseXY))
		return ;
	WriteProcessMemory(m_hprocess, (BYTE*)(CBaseXY + offsets::dwClientState_ViewAngles), AimbotAngle, 12, 0);

}

/* demo
void CHack::GlowESP()
{
	// glow
	const auto glowObjectManager = memory::read<DWORD>(m_hprocess, (DWORD)m_hclient + offsets::dwGlowObjectManager);

	printf("%x \n", glowObjectManager);
	for (auto i = 1; i <= 32; ++i)
	{
		const auto entity = memory::read<std::uintptr_t>(m_hprocess, (DWORD)m_hclient + offsets::entity_list + i * 0x10);

		if (!entity)
			continue;

		// dont glow if they are on our team
		memory::read<std::int32_t>(m_hprocess, (DWORD)entity + offsets::m_iGlowIndex);
		if (memory::read<DWORD>(m_hprocess, (DWORD)entity + offsets::team_num) == g_Local.m_iTeam)
			continue;

		const auto glowIndex = memory::read<std::int32_t>(m_hprocess, (DWORD)entity + offsets::m_iGlowIndex);

		GlowStruct currentGlow = memory::read<GlowStruct>(m_hprocess,glowObjectManager + glowIndex * 0x38);
		currentGlow.Red = 1.f;
		currentGlow.Green = 0.f;
		currentGlow.Blue = 0.f;

		currentGlow.Alpha = 1.f;
		currentGlow.renderOccluded = true;
		currentGlow.renderUnoccluded = false;

		memory::write<GlowStruct>(m_hprocess,glowObjectManager + (glowIndex * 0x38), currentGlow);

		// do glow by writing each variable
		//mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x8, 1.f);
		//mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0xC, 0.f);
		//mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x10, 0.f);
		//mem.Write<float>(glowObjectManager + (glowIndex * 0x38) + 0x14, 1.f);

		//mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
		//mem.Write<bool>(glowObjectManager + (glowIndex * 0x38) + 0x29, true);

	}
	
}
*/
void CHack::GlowESP()
{
	if (vars::visuals::glow == false)
		return;

	const auto glowObjectManager = memory::read<DWORD>(m_hprocess, (DWORD)m_hclient + offsets::dwGlowObjectManager);

	for (auto p : this->g_Players)
	{
		if (vars::visuals::player_type == 0 && p.m_iTeam == this->g_Local.m_iTeam)
			continue;

		if (vars::visuals::player_type == 2)
			return;
		ImColor col_box = PlayerColor(p.m_iTeam);

		GlowStruct currentGlow = memory::read<GlowStruct>(m_hprocess, glowObjectManager + p.m_glowIndex * 0x38);

		currentGlow.Red = col_box.Value.x;
		currentGlow.Green = col_box.Value.y;
		currentGlow.Blue = col_box.Value.z;

		currentGlow.Alpha = 1.f;
		currentGlow.renderOccluded = true;
		currentGlow.renderUnoccluded = false;

		memory::write<GlowStruct>(m_hprocess, glowObjectManager + (p.m_glowIndex * 0x38), currentGlow);
	}
}
