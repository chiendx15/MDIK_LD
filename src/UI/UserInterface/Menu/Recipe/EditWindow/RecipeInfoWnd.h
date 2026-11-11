#pragma once

class CRecipeInfoWnd : public CWnd, public CIndividualRecipe
{
	// 레시피 정보 리스트 항목
	enum eRECIPE_INFO_COLUMMN
	{
		eRIC_NONE,
		eRIC_NAME,		// 이름 (항목 명)
		eRIC_VALUE,		// 값
		eRIC_UNIT,		// 단위
	};

	DECLARE_DYNAMIC(CRecipeInfoWnd)
public:
	CRecipeInfoWnd(bool& _editAble, T_MDI_RECIPE& _recipe, int _nameWidth = 250, int _valueWidth = 80, int _unitWidth = 63);
	virtual ~CRecipeInfoWnd();

private:
	bool&			m_editAble;
	T_MDI_RECIPE&	m_recipe;
	// 컬럼 사이즈
	int				m_nameWidth = 0;
	int				m_valueWidth = 0;
	int				m_unitWidth = 0;


	int				m_itemCount = 0;
	CListCtrlColumnEx	m_infoList;
	std::vector<std::function<void(bool)>> m_infoSyncFunctions;
private:
	template <typename T>
	void SyncRecipeInfo(bool _toList, int _idx, const CString& _name, T& _value, const CString& _unit = _T(""))
	{
		if (!m_infoList.m_hWnd)
			return;

		if (_toList)
		{
			int count = m_infoList.GetItemCount();
			if (count <= _idx)
			{
				m_infoList.InsertItem(_idx, _T(""));
			}
			m_infoList.SetItemBKColor(_idx, _idx % 2 ? RGB(255, 255, 255) : RGB(230, 230, 250));
			m_infoList.SetItemText(_idx, eRIC_NAME, _name);
			m_infoList.SetItemText(_idx, eRIC_VALUE, ToCString(_value));
			m_infoList.SetItemText(_idx, eRIC_UNIT, _unit);
		}
		else
		{
			assert(m_infoList.GetItemCount() >= _idx);
			CString value = m_infoList.GetItemText(_idx, eRIC_VALUE);
			_value = FromCString<T>(value);

			// double 데이터 같은 경우 소수 점 단위 %.3f 까지 표현되어야하는데 
			// 유저가 입력 안하면 짤리는 현상을 막기 위해서 ... (그냥 입력 안한 값 자체로 표현하면 좋은데...)
			m_infoList.SetItemText(_idx, eRIC_VALUE, ToCString(_value));
		}
	}

public:
	template <typename T>
	void InsertItem(const CString& _name, T& _value, const CString& _unit = _T(""), std::function<void()> _func = nullptr)
	{
		int idx = m_itemCount++;
		m_infoSyncFunctions.emplace_back
		(
			[&, idx, _name, _unit, _func](bool _toListCtrl) -> void
			{
				SyncRecipeInfo(_toListCtrl, idx, _name, _value, _unit);
				if (_func != nullptr)
				{ 
					_func();
				}
			}
		);
	}

public:
	void LoadInitialize() override;
	void ChangeEditColor() override;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnLvnInfoBeginEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnInfoEndEdit(NMHDR *pNMHDR, LRESULT *pResult);
};


