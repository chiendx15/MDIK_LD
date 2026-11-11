#pragma once

#include <array>
#include <vector>
#include <list>

#include "misc/Profile.h"
#include "misc/ListCtrlEd.h"
#include "misc/ListCtrlColumnEx.h"
#include "RecipeData.h"



template <typename RECIPE>
class CRecipe
{
	inline static wchar_t error_words[] = {L':', L':', L'*', L'?', L'<', L'>', L'|'};

public:
	CRecipe()
	{
		SetDirectoryPath(m_recipeDirPath);
		m_recipesInfo = CProfile(m_recipeDirPath + _T("RecipeInfo.ini"));
	}
protected: 
	CString			m_recipeDirPath = _T("../data/recipe/"); // 저장/불러오기 할 폴더 정보
	CString			m_extension = _T(".json"); // 레시피 확장자	// KES 수정 2024.2.19 .rcp -> .json

	std::array<std::unique_ptr<RECIPE>, 99>	m_recipes;
	CListCtrlColumnEx*					m_listCtrl = nullptr;

private:
	CProfile	m_recipesInfo; // 레시피 리스트 용 데이터 < 적용 상태, 이름, 설명 > (이외 데이터는 각각의 파일에 저장)
	int			m_appliedIndex = -1;

public:
	void SetListCtrl(CListCtrlColumnEx* _listCtrl)
	{
		m_listCtrl = _listCtrl;
		
		if (m_listCtrl == nullptr)
			return;

		m_listCtrl->DeleteAllItems();
		for (int idx = 0; idx < m_recipes.size(); ++idx)
		{
			//m_listCtrl->InsertItem(idx, _T(""));
			LVITEM lvItem;
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = idx;
			lvItem.iSubItem = 0;
			lvItem.pszText = L"";
			COLORREF bk = idx % 2 ? RGB_WHITE : RGB(230, 230, 250);
			m_listCtrl->InsertItem(&lvItem, RGB_BLACK, bk);
		}
	}

	bool SetDirectoryPath(CString _dirPath)
	{
		_dirPath.Trim();
		if (_dirPath.IsEmpty())
			return false;
		
		for (auto& word : error_words)
		{
			if (_dirPath.Find(word) != -1)
				return false;
		}

		m_recipeDirPath = _dirPath;
		CreateSubDirectories(m_recipeDirPath);
		return true;
	}

	CString GetDirectoryPath() const 
	{ 
		return m_recipeDirPath; 
	}

	bool SetExtension(CString _extension)
	{
		_extension.Trim();
		if (_extension.IsEmpty())
			return false;

		for (auto& word : error_words)
		{
			if (_extension.Find(word) != -1)
				return false;
		}

		if (_extension.GetAt(0) != L'.')
		{
			_extension.Insert(0, L'.');
		}

		m_extension = _extension;
		return true;
	}

	CString GetExtension() const
	{
		return m_extension;
	}

	const RECIPE* GetAppliedData() const	
	{ 
		if  (m_appliedIndex == -1)
			return nullptr;

		return m_recipes[m_appliedIndex].get();
	}

	RECIPE* GetRecipeData(int iRecipe) const
	{
		if (iRecipe == -1 || iRecipe >= m_recipes.size())
			return nullptr;

		return m_recipes[iRecipe].get();
	}

	RECIPE* GetAppliedData()
	{ 
		if  (m_appliedIndex == -1)
			return nullptr;

		return m_recipes[m_appliedIndex].get();
	}

	RECIPE* CreateRecipe(int _idx)
	{
		if (m_appliedIndex == _idx)
			return nullptr;

		// to do message box or delete
		if (m_recipes[_idx].get() != nullptr)
		{
			TRACE("org recipe[%d] delete after create new recipe[%d]\n", _idx);
		}
		else
		{
			TRACE("create new recipe[%d]\n", _idx);
		}

		CString name = newRecipeName();
		m_recipes[_idx].reset(new RECIPE);
		m_recipes[_idx]->recipeName = name;
		
		saveRecipe(_idx);

		return m_recipes[_idx].get();
	}
	
	RECIPE* CreateRecipe()
	{
		if (m_listCtrl == nullptr)
			return nullptr;

		//int selectItem = m_listCtrl->GetSelectedItem();
		int selectItem = m_listCtrl->GetSelectionMark();
		if (selectItem == -1)
			return nullptr;

		return CreateRecipe(selectItem);
	}

	RECIPE* CopyRecipe(T_MDI_RECIPE* _orgData, int _copyIdx)
	{
		if (_orgData == nullptr)
			return nullptr;

		CString name = _orgData->recipeName;
		name = newRecipeName(name);

		auto& copyRecipe = m_recipes[_copyIdx];
		copyRecipe.reset(new RECIPE);
		*(copyRecipe.get()) = *_orgData;
		copyRecipe->recipeName = name;
		copyRecipe->applied = false;
		copyRecipe->cimapplied = false;
		copyRecipe->modifiedCount = 0;
		copyRecipe->ppid = L"";
		setListData(_copyIdx);

		saveRecipe(_copyIdx);

		return copyRecipe.get();
	}

	RECIPE* CopyRecipe(T_MDI_RECIPE* _orgData)
	{
		if (m_listCtrl == nullptr)
			return nullptr;

		int selectItem = m_listCtrl->GetSelectedItem();
		if (selectItem == -1)
			return nullptr;

		return CopyRecipe(_orgData, selectItem);
	}

	RECIPE* CopyRecipe(int _selectIdx, int _copyIdx)
	{
		return CopyRecipe(m_recipes[_selectIdx].get(), _copyIdx);
	}

	RECIPE* CopyRecipe()
	{
		if (m_listCtrl == nullptr || m_appliedIndex == -1)
			return nullptr;

		//int selectItem = m_listCtrl->GetSelectedItem();
		int selectItem = m_listCtrl->GetSelectionMark();
		if (selectItem == -1 || m_appliedIndex == selectItem)
		{
			// to do Message box
			TRACE(_T("Copy Index와 Applied Index가 같다"));
			return nullptr;
		}

		return CopyRecipe(m_appliedIndex, selectItem);
	}

	void DeleteRecipe(int _selectIdx)
	{
		if (m_appliedIndex == _selectIdx)
			return;

		// 데이터 삭제
		deleteRecipe(_selectIdx);		
	}

	void DeleteRecipe()
	{
		//int selectItem = m_listCtrl->GetSelectedItem();
		int selectItem = m_listCtrl->GetSelectionMark();
		if (selectItem == -1)
			return;

		DeleteRecipe(selectItem);
	}
	
	bool ApplyRecipe()
	{
		if (m_listCtrl == nullptr)
			return false;

		//int selectIndex = m_listCtrl->GetSelectedItem();
		int selectIndex = m_listCtrl->GetSelectionMark();
		if (selectIndex == -1 || m_recipes[selectIndex] == nullptr || m_appliedIndex == selectIndex)
		{
			return false;
		}
		
		if (m_appliedIndex != -1)
		{
			m_recipes[m_appliedIndex]->applied = false;
			m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), m_appliedIndex + 1), _T("Apply"), m_recipes[m_appliedIndex]->applied);
			//m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), m_appliedIndex + 1), _T("CimApply"), m_recipes[m_appliedIndex]->cimapplied);
			setListData(m_appliedIndex);
		}

		m_appliedIndex = selectIndex;
		loadRecipe(selectIndex);
		m_recipes[m_appliedIndex]->applied = true;
		m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), m_appliedIndex + 1), _T("Apply"), m_recipes[m_appliedIndex]->applied);
		//m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), m_appliedIndex + 1), _T("CimApply"), m_recipes[m_appliedIndex]->cimapplied);
		setListData(m_appliedIndex);

		if (!m_recipesInfo.Save())
			return false;

		return true;
	}

	bool SaveAppliedRecipe()
	{
		//int selectItem = m_listCtrl->GetSelectedItem();
		//if (selectItem == -1)
		//	return false;
		//return saveRecipe(selectItem);
		if (m_appliedIndex == -1)
			return false;

		return saveRecipe(m_appliedIndex);
	}
	
	// 처음 프로그램 로딩 때만.. 평소에는 사용 X
	bool LoadRecipeList()
	{
		// file finder 소멸 시점에 close 호출 후 삭제
		std::unique_ptr<CFileFind, std::function<void(CFileFind*)>> ff
		(
			new CFileFind,
			[](CFileFind* p)
			{
				p->Close();
				delete p;
			}
		);

		m_appliedIndex = -1;
		int recipeSize = m_recipesInfo.GetSections().N();
		for (int idx = 0; idx < m_recipes.size(); ++idx)
		{
			resetListData(idx);
		}

		for (int idx = 0; idx < recipeSize; ++idx)
		{
			CString sectionName = m_recipesInfo.GetSection(idx).GetName();
			int indexPos = sectionName.Find(_T("_"));
			int selectRecipe = _ttoi(sectionName.Mid(indexPos + 1)) - 1;

			if (selectRecipe == -1)
				return false;

			if (m_recipes.size() <= selectRecipe)
				continue;

			auto& recipe = m_recipes[selectRecipe];
			recipe.reset(new RECIPE);

			m_recipesInfo.SyncProfileValue(false, sectionName, _T("Name"), recipe->recipeName);
			m_recipesInfo.SyncProfileValue(false, sectionName, _T("Ppid"), recipe->ppid);
			m_recipesInfo.SyncProfileValue(false, sectionName, _T("Apply"), recipe->applied);
			m_recipesInfo.SyncProfileValue(false, sectionName, _T("CimApply"), recipe->cimapplied);
			m_recipesInfo.SyncProfileValue(false, sectionName, _T("PannelSize"), recipe->pannelSize);
			m_recipesInfo.SyncProfileValue(false, sectionName, _T("ModifiedCount"), recipe->modifiedCount);
			m_recipesInfo.SyncProfileValue(false, sectionName, _T("Lastupdate Time"), recipe->lastupdateTime);
			m_recipesInfo.SyncProfileValue(false, sectionName, _T("Description"), recipe->description);

			// 리스트 컨트롤러 업데이트
			setListData(selectRecipe);

			// apply 레시피 데이터는 로드 함
			if (1 || recipe->applied)
			{
				if (recipe->applied)
					m_appliedIndex = selectRecipe;

				//<< KES 수정 2024.2.19 - json 형식으로 파일 저장
				bool bBeforeCIMApply = recipe->cimapplied;
				auto ppid = recipe->ppid;

				CString path = m_recipeDirPath + recipe->recipeName + m_extension;
				
				json j;
				std::ifstream in(path);
				if (in.is_open()) {
				  in >> j;
				  *recipe = j;
				}
				else {
					AfxMessageBox(Format(_T("%s: (%s.json). (%s)"), g_st[IDS_RECIPE_FOLLOWING_RECIPE_FILE_MISSING],recipe->recipeName, g_st[IDS_RECIPE_SETTING_TO_BASIC_DATA]));
					continue;
				}
				//>>

				recipe->applied = recipe->applied;
				recipe->cimapplied = bBeforeCIMApply;
				recipe->ppid = ppid;
			}
		}

		return true;
	}


	std::list<std::pair<int, CString>> SearchRecipe(const CString& _word) const
	{
		std::list<std::pair<int, CString>> SearchList;

		for (int idx = 0; idx < m_recipes.size(); ++idx)
		{
			auto& recipe = m_recipes[idx];
			if (recipe.get() == nullptr)
				continue;
			
			if (FindNoCase(recipe->recipeName, _word) == -1)
				continue;
			
			SearchList.push_back({idx + 1, recipe->recipeName});
		}

		return SearchList;
	}

	size_t Size() const { return m_recipes.size(); }
	bool IsEmpty() const { return m_recipes.empty(); }
	int GetAppliedIndex() const { return m_appliedIndex; }

	RECIPE*		operator[](int _index)			{ return m_recipes[_index].get(); }
	const RECIPE*	operator[](int _index) const	{ return m_recipes[_index].get(); }

// to do change (현재 테스트용으로 public 해둠)
public:
//private:
	void setListData(const T_MDI_RECIPE* _data, int _index)
	{
		if (m_listCtrl == nullptr)
			return;

		m_listCtrl->SetItemText(_index, eRC_INDEX, _data->applied ? Format(_T("<<%d>>"), _index + 1) : Format(_T("%d"), _index + 1));
		m_listCtrl->SetItemText(_index, eRC_RECIPE_ID, _data->recipeName);
		m_listCtrl->SetItemText(_index, eRC_PPID, _data->cimapplied ? Format(_T("<<%s>>"), _data->ppid) : Format(_T("%s"), _data->ppid));
		m_listCtrl->SetItemText(_index, eRC_PANNEL_SIZE, Format(_T("%.3f x %.3f mm"), _data->pannelSize.cx, _data->pannelSize.cy));
		m_listCtrl->SetItemText(_index, eRC_MODIFIED_COUNT, Format(_T("%d"), _data->modifiedCount));
		m_listCtrl->SetItemText(_index, eRC_LASTUPDATE_TIME, _data->lastupdateTime);
		m_listCtrl->SetItemText(_index, eRC_DESCRIPTION, _data->description);
	}

	void setListData(int _index)
	{
		setListData(m_recipes[_index].get(), _index);
	}

	BOOL setRecipeName(int _index, CString strName) {
		if (m_recipes[_index].get() == nullptr)
			return FALSE;

		m_recipes[_index]->recipeName = strName;
		saveRecipe(_index);
		return TRUE;
	}	
	
	BOOL setCIMApplyRecipe(int _index) {
		if (m_recipes[_index].get() == nullptr)
			return FALSE;

		m_recipes[_index]->cimapplied = true;
		saveRecipe(_index);
		return TRUE;
	}

	void resetListData(int _index)
	{
		if (m_listCtrl == nullptr)
			return;

		m_listCtrl->SetItemText(_index, eRC_INDEX, Format(_T("%d"), _index + 1));
		m_listCtrl->SetItemText(_index, eRC_RECIPE_ID, _T(""));
		m_listCtrl->SetItemText(_index, eRC_PPID, _T("")); // 2022.03.11 레시피 삭제할 때 PPID 삭제 안되는 문제 수정
		m_listCtrl->SetItemText(_index, eRC_PANNEL_SIZE, _T(""));
		m_listCtrl->SetItemText(_index, eRC_MODIFIED_COUNT, _T(""));
		m_listCtrl->SetItemText(_index, eRC_LASTUPDATE_TIME, _T(""));
		m_listCtrl->SetItemText(_index, eRC_DESCRIPTION, _T(""));
	}

	
	bool saveRecipe(int _index)
	{
		auto& recipe = m_recipes[_index];
		if (recipe == nullptr)
		{
			assert(false); // 없는데 저장해?
		}

		CString fileName;
		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("Name"), fileName);
		
		std::unique_ptr<CFileFind, std::function<void(CFileFind*)>> ff
		(
			new CFileFind,
			[](CFileFind* p)
			{
				p->Close();
				delete p;
			}
		);

		// 저장 전 기존 레시피 파일 삭제
		if (ff->FindFile(m_recipeDirPath + fileName + m_extension))
		{
			DeleteFile(m_recipeDirPath + fileName + m_extension);
		}
		
		// 저장 전 현재 시간 업데이트
		CTime cTime = CTime::GetCurrentTime(); 
		recipe->lastupdateTime.Format
		(
			_T("%04d-%02d-%02d %02d:%02d:%02d"), 
			cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), 
			cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond()
		);

		// KES 수정 2024.1.23 - json 형식으로 파일 저장
		CString path = m_recipeDirPath + recipe->recipeName + m_extension;
		json j;
		j = *recipe;
		std::ofstream out(path);
		if (out.is_open()) {
		  out << j;
		}

		// 레시피 리스트 목록에 바뀐 내용 업데이트
		m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), _index + 1), _T("Name"), recipe->recipeName);
		m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), _index + 1), _T("Ppid"), recipe->ppid);
		m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), _index + 1), _T("Apply"), recipe->applied);
		m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), _index + 1), _T("CimApply"), recipe->cimapplied);
		m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), _index + 1), _T("PannelSize"), recipe->pannelSize);
		m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), _index + 1), _T("ModifiedCount"), recipe->modifiedCount);
		m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), _index + 1), _T("Lastupdate Time"), recipe->lastupdateTime);
		m_recipesInfo.SyncProfileValue(true, Format(_T("Recipe_%d"), _index + 1), _T("Description"), recipe->description);

		if (!m_recipesInfo.Save())
		{
			return false;
		}

		setListData(_index);

		return true;
	}

	bool loadRecipe(int _index)
	{
		auto& recipe = m_recipes[_index];

		recipe.reset(new RECIPE);		
		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("Name"), recipe->recipeName);
		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("Ppid"), recipe->ppid);
		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("Apply"), recipe->applied);
		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("CimApply"), recipe->cimapplied);
		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("PannelSize"), recipe->pannelSize);
		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("ModifiedCount"), recipe->modifiedCount);
		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("Lastupdate Time"), recipe->lastupdateTime);
		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("Description"), recipe->description);

		std::unique_ptr<CFileFind, std::function<void(CFileFind*)>> ff
		(
			new CFileFind,
			[](CFileFind* p)
			{
				p->Close();
				delete p;
			}
		);

		//<< KES 추가 2024.1.23 - json 형식으로 파일 저장
		bool bBeforeCIMApply = recipe->cimapplied;
		auto ppid = recipe->ppid;

		CString path = m_recipeDirPath + recipe->recipeName + m_extension;
		json j;
		std::ifstream in(path);
		if (in.is_open()) {
		  in >> j;
		  *recipe = j;
		}
		else {
			AfxMessageBox(Format(_T("%s: (%s.json). (%s)"), g_st[IDS_RECIPE_FOLLOWING_RECIPE_FILE_MISSING],recipe->recipeName, g_st[IDS_RECIPE_SETTING_TO_BASIC_DATA]));
			return false;
		}
		//>>

		recipe->cimapplied = bBeforeCIMApply;
		recipe->ppid = ppid;

		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("CimApply"), recipe->cimapplied);
		// 리스트 컨트롤러 업데이트
		setListData(_index);
		return true;
	}

	bool deleteRecipe(int _index)
	{
		if (m_appliedIndex == _index)
			return false;

		auto& recipe = m_recipes[_index];
		//<< 2024.03.27 kms - 비어있는 레시피 삭제하면 포인터 없어서 뒤짐
		if (!recipe)
			return true;
		//>>

		if (recipe->applied)
		{
			return false;
		}
		recipe.reset();

		CString fileName;
		m_recipesInfo.SyncProfileValue(false, Format(_T("Recipe_%d"), _index + 1), _T("Name"), fileName);

		std::unique_ptr<CFileFind, std::function<void(CFileFind*)>> ff
		(
			new CFileFind,
			[](CFileFind* p)
			{
				p->Close();
				delete p;
			}
		);

		// 저장 전 기존 레시피 파일 삭제
		if (ff->FindFile(m_recipeDirPath + fileName + m_extension))
		{
			DeleteFile(m_recipeDirPath + fileName + m_extension);
		}
		resetListData(_index);

		m_recipesInfo.DeleteSection(Format(_T("Recipe_%d"), _index + 1));
		if (!m_recipesInfo.Save())
		{
			return false;
		}

		return true;
	}

	CString newRecipeName(CString _name = _T("NewRecipe"))
	{
		int count = 0;
		CString name(_name);
		for (int idx = 0; idx < m_recipes.size(); ++idx)
		{
			if (m_recipes[idx] == nullptr)
				continue;

			if (m_recipes[idx]->recipeName.Compare(name) == 0)
			{
				name.Format(_T("%s_%d"), _name, count++);
				idx = -1;
			}
		}

		return name;
	}

	bool syncData(bool _store, CString _section, RECIPE* _recipe)
	{
		return false;
	}
};


class CIndividualRecipe
{
public:
	virtual void LoadInitialize() = 0;
	virtual void ChangeEditColor() = 0;
};


class T_SCRIBE_PATTERN_DATA_RECIPE
{
public:
	int iImageIndex = -1;
	CString description = _T("");
	double dLeftDistanceX = 0.;			// 패널 좌측에서 마크까지 X 거리
	double dRightDistanceX = 0.;		// 패널 우측에서 마크까지 X 거리
	double dPitchDistanceY = 0.;		// 가공 라인에서 마크까지 Y 거리

public:
	FEM_BEGIN_TBL(T_SCRIBE_PATTERN_DATA_RECIPE)
		FEM_ADD_MEMBER(iImageIndex)
		FEM_ADD_MEMBER(description)
		FEM_ADD_MEMBER(dLeftDistanceX)
		FEM_ADD_MEMBER(dRightDistanceX)
		FEM_ADD_MEMBER(dPitchDistanceY)
		FEM_END_TBL()
		FEM_SERIALIZE(T_SCRIBE_PATTERN_DATA_RECIPE)
};
