
#include "stdafx.h"

/*
//--------------------------------------------------------------------------------
int SS_RowEffectivelySelected(StudySelector *ss, CSSListRow* pRow)
	{
	if(pRow->IsSelected())
		return 1;

	while(pRow->up)
		{
		pRow = pRow->up;

		if(pRow->IsSelected())
			return 1;
		}

	return 0;
	}

//--------------------------------------------------------------------------------
CSSListRow *SS_NextLogicalRow(StudySelector *ss, CSSListRow *source)
	{
	if(source == NULL)
		return NULL;
	
	if(source->down)
		return source->down;

	if(source->HasSibling())
		return source->GetSibling();

	while(source->up)
		{
		source = source->up;
		if(source->HasSibling())
			return source->GetSibling();
		}
	return NULL;
	}

//--------------------------------------------------------------------------------
CSSListRow *SS_NextImage(CSSListRow* source)
	{
	if(!source)
		return(NULL);

	if(source->layer!=SS_IMAGE_LAYER)
		{
		while(source && source->layer!=SS_IMAGE_LAYER)
			source=source->down;

		return(source);
		}
	else if(source->GetSibling())
		return(source->GetSibling());
	else
		{
		while(source->up)
			{
			source=source->up;
			if(source->GetSibling())
				return(SS_NextImage(source->GetSibling()));
			}
		}
	return(NULL);
	}

//--------------------------------------------------------------------------------
CSSListRow *SS_NextSelectedRow(StudySelector *ss, CSSListRow *source)
	{
	if(!source)
		return NULL;
	while(source)
		{
		if(source->down)
			{
			if(source->down->IsSelected())
				return source->down;
			else
				{
				source = source->down;
				continue;
				}
			}
		if(source->GetSibling())
			{
			if(source->GetSibling()->IsSelected())
				return source->GetSibling();
			else
				{
				source = source->GetSibling();
				continue;
				}
			}
		while(source)
			{
			source = source->up;
			if(source && source->GetSibling())
				{
				if(source->GetSibling()->IsSelected())
					return source->GetSibling();
				else
					{
					source = source->GetSibling();
					break;
					}
				}
			}
		}
	return NULL;
	}

//--------------------------------------------------------------------------------
inline CSSListRow* SS_FirstSelectedRow(StudySelector *ss)
	{
	return (ss->first_row != NULL && ss->first_row->IsSelected()) ? ss->first_row : SS_NextSelectedRow(ss, ss->first_row);
	}

//--------------------------------------------------------------------------------
CSSListRow *SS_NextLayerRow(StudySelector *ss, CSSListRow *source, SS_AttributeLayer layer)
	{
	while(source)
		{
		if((source->layer != layer) && source->down)
			if(source->down->layer == layer)
				return source->down;
			else
				{
				source = source->down;
				continue;
				}
			if(source->GetSibling())
				if(source->GetSibling()->layer == layer)
					return source->GetSibling();
				else
					{
					source = source->GetSibling();
					continue;
					}
				while(source)
					{
					source = source->up;
					if(source && source->GetSibling())
						if(source->GetSibling()->layer == layer)
							return source->GetSibling();
						else
							{
							source = source->GetSibling();
							break;
							}
					}
		}
	return NULL;
	}

//--------------------------------------------------------------------------------
inline CSSListRow* SS_FirstLayerRow(StudySelector *ss, SS_AttributeLayer srch_layer)
	{
	return (ss->first_row != NULL && ss->first_row->layer == srch_layer) ? ss->first_row :
		SS_NextLayerRow(ss, ss->first_row, srch_layer);
	}

//--------------------------------------------------------------------------------
CSSListRow *SS_FirstSelectedImage(StudySelector *ss)
	{

	if(!ss->first_row)
		return NULL;
	return SS_NextSelectedImage(ss, ss->first_row);
	}

//--------------------------------------------------------------------------------
CSSListRow *SS_NextSelectedImage(StudySelector *ss, CSSListRow *pSource)
	{
	if(pSource == NULL)
		return NULL;

	while(pSource != NULL)
		{
		if(pSource->down != NULL)
			// Down is selected image row
			if(pSource->down->down == NULL && SS_RowEffectivelySelected(ss, pSource->down))
				return pSource->down;
			else
				{
				pSource = pSource->down;
				continue;
				}

			if(pSource->GetSibling() != NULL)
				// Down is selected image row
				if(pSource->GetSibling()->down == NULL && SS_RowEffectivelySelected(ss, pSource->GetSibling()))
					return pSource->GetSibling();
				else
					{
					pSource = pSource->GetSibling();
					continue;
					}

				while(pSource != NULL)
					{
					pSource = pSource->up;
					if(pSource != NULL && pSource->GetSibling() != NULL)
						// Down is selected image row
						if(pSource->GetSibling()->down == NULL && SS_RowEffectivelySelected(ss, pSource->GetSibling()))
							return pSource->GetSibling();
						else
							{
							pSource = pSource->GetSibling();
							break;
							}
					}
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
CSSListRow *SS_NextDisplayedRow(StudySelector *ss, CSSListRow *source)
	{
	if(!source)
		return NULL;
	if(source->down && source->expanded)
		return source->down;
	if(source->GetSibling())
		return source->GetSibling();
	while(source->up)
		{
		source = source->up;
		if(source->GetSibling())
			return source->GetSibling();
		}
	return NULL;
	}

//--------------------------------------------------------------------------------
CSSListRow *SS_RowNAfterRow(StudySelector *ss, CSSListRow *pSource, int nWhich)
	{
	for(int i = 0; i < nWhich; i++)
		pSource = SS_NextDisplayedRow(ss, pSource);

	return pSource;
	}

*/