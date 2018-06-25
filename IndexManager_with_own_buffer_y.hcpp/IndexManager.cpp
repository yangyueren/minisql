#include "IndexManager.h"

using namespace std;
IndexManager im;

IndexManager::IndexManager()
{
	IndexNum = 1;
	struct Index temp;
	temp.IndexFileName = "";
	temp.Type = -1;
	IndexSet.push_back(temp);
}

IndexManager::~IndexManager()
{

}

bool IndexManager::GetIndex(string IndexName, int Key_Type)
{
	struct Index temp_index;

	temp_index.Type = Key_Type;
	temp_index.IndexFileName = IndexName;
	temp_index.B_Plus_Tree_int.BPlusTree_name = IndexName;
	if(Key_Type == INT_TYPE)
		GetIndexHead(&temp_index.B_Plus_Tree_int);
	else if(Key_Type == FLOAT_TYPE)
		GetIndexHead(&temp_index.B_Plus_Tree_float);
	else if(Key_Type == STRING_TYPE)
		GetIndexHead(&temp_index.B_Plus_Tree_string);
	else
		return false;

	//IndexSet.insert(IndexSet.begin()+IndexNum, temp_index);
	IndexSet.insert(IndexSet.begin(), temp_index);
	//IndexNum++;
	return true;
}

bool IndexManager::DeleteIndex(string IndexName)
{
	int i;
	int index = -1;

	for(i=0; i<IndexNum; i++)
		if(IndexName == IndexSet[i].IndexFileName)
		{
			index = i;
			break;
		}

	if(index == -1)
	{
		//cout<<"No such index named "<<IndexName<<" ,Deletion failed!"<<endl;
		return false;
	}
	else
	{
		IndexSet.erase(IndexSet.begin()+index);
		//IndexNum--;
		//cout<<"Index "<<IndexName<<" has been deleted!"<<endl;
		return true;
	}
}

bool IndexManager::CreateIndex(string IndexName, int KeySize, int Key_Type, int Degree)
{
	
	if(Key_Type == INT_TYPE)
	{
		BPlusTree<int> BPT(IndexName, KeySize, Key_Type, Degree);
		createNewBlock(IndexName);
		UpdateIndex(&BPT);
		BPT.LeafHead = BPT.root = createNewBlock(IndexName);
		UpdateIndex(&BPT);
		Node<int> node(KeySize);
		node.Self = BPT.root;
		node.key_num = 0;
		node.IsLeaf = true;
		BPT.UpdateNode(node, node.Self);
	}
	else if(Key_Type == FLOAT_TYPE)
	{
		BPlusTree<float> BPT(IndexName, KeySize, Key_Type, Degree);
		BPT.LeafHead = BPT.root = createNewBlock(IndexName);
		UpdateIndex(&BPT);
		Node<float> node(KeySize);
		node.Self = BPT.root;
		node.key_num = 0;
		node.IsLeaf = true;
		BPT.UpdateNode(node, node.Self);
	}
	else if(Key_Type == STRING_TYPE)
	{
		BPlusTree<string> BPT(IndexName, KeySize, Key_Type, Degree);
		BPT.LeafHead = BPT.root = createNewBlock(IndexName);
		UpdateIndex(&BPT);
		Node<string> node(KeySize);
		node.Self = BPT.root;
		node.key_num = 0;
		node.IsLeaf = true;
		BPT.UpdateNode(node, node.Self);
	}
	else
		return false;

	bm_y.writeBackDisk(IndexName);

	return true;
}


bool IndexManager::CreateIndex(string IndexName, int KeySize, int Key_Type)
{
	int Degree;

	//Degree = 1000/KeySize;
	Degree = 5;
	return CreateIndex(IndexName, KeySize, Key_Type, Degree);
}


OffsetType IndexManager::SearchInIndex(string IndexName, string KeyValue, int Key_Type)
{
	int i;
	int index = -1;
	OffsetType offset = -1;
	OffsetType data_offset = -1;
	bool exist = false;
	if (IndexSet[0].IndexFileName != IndexName)
	{
		if (IndexSet[0].IndexFileName != "")
		{
			bm_y.writeBackDisk(IndexSet[0].IndexFileName);
			DeleteIndex(IndexSet[0].IndexFileName);
		}
		bm_y.loadToArray(IndexName);
		GetIndex(IndexName, Key_Type);

	}

	for(i=0; i<IndexNum; i++)
		if(IndexName == IndexSet[i].IndexFileName)
		{
			index = i;
			break;
		}

	if(index == -1)
	{
		//cout<<"No such index named "<<IndexName<<" ,Search failed!"<<endl;
		return -1;
	}
	else
	{
		if(Key_Type == INT_TYPE)
		{
			exist = IndexSet[i].B_Plus_Tree_int.Search(stoi(KeyValue), offset);
			Node<int> node;
			IndexSet[i].B_Plus_Tree_int.GetNode(node, offset);
			data_offset = IndexSet[i].B_Plus_Tree_int.Get_Data_Address(stoi(KeyValue), node);
		}
		else if(Key_Type == FLOAT_TYPE)
		{
			exist = IndexSet[i].B_Plus_Tree_float.Search(stof(KeyValue), offset);
			Node<float> node;
			IndexSet[i].B_Plus_Tree_float.GetNode(node, offset);
			data_offset = IndexSet[i].B_Plus_Tree_float.Get_Data_Address(stof(KeyValue), node);
		}
		else if(Key_Type == STRING_TYPE)
		{
			exist = IndexSet[i].B_Plus_Tree_string.Search(KeyValue, offset);
			Node<string> node;
			IndexSet[i].B_Plus_Tree_string.GetNode(node, offset);
			data_offset = IndexSet[i].B_Plus_Tree_string.Get_Data_Address(KeyValue, node);
		}
		else
			exist = false;
	}

	if(exist)
		return data_offset;
	else
		return -1;
}

bool IndexManager::InsertIntoIndex(string IndexName, string KeyValue, int Key_Type, OffsetType Offset)
{
	int i;
	int index = -1;
	bool succeed = false;
	bool exist = false;
	//OffsetType offset;
	if (IndexSet[0].IndexFileName != IndexName)
	{

	
		if (IndexSet[0].IndexFileName != "")
		{
			bm_y.writeBackDisk(IndexSet[0].IndexFileName);
			DeleteIndex(IndexSet[0].IndexFileName);
		}
		bm_y.loadToArray(IndexName);
		GetIndex(IndexName, Key_Type);

		
	/*	bm_y.loadToArray(IndexName);
		GetIndex(IndexName, Key_Type);*/
	}

	for(i=0; i<IndexNum; i++)
		if(IndexName == IndexSet[i].IndexFileName)
		{
			index = i;
			break;
		}

	if(Key_Type == INT_TYPE)
		succeed = IndexSet[i].B_Plus_Tree_int.Insert(stoi(KeyValue), Offset);
	else if(Key_Type == FLOAT_TYPE)
		succeed = IndexSet[i].B_Plus_Tree_float.Insert(stof(KeyValue), Offset);
	else if(Key_Type == STRING_TYPE)
		succeed = IndexSet[i].B_Plus_Tree_string.Insert(KeyValue, Offset);
	else
		succeed = false;
	//}

	return succeed;
}





bool IndexManager::DeleteInIndex(string IndexName, string KeyValue, int Key_Type)
{
	int i;
	int index = -1;
	OffsetType offset = -1;
	bool succeed = false;
	bool exist = false;
	if (IndexSet[0].IndexFileName != IndexName)
	{
		if (IndexSet[0].IndexFileName != "")
		{
			bm_y.writeBackDisk(IndexSet[0].IndexFileName);
			DeleteIndex(IndexSet[0].IndexFileName);
		}
		bm_y.loadToArray(IndexName);
		GetIndex(IndexName, Key_Type);

	}

	for(i=0; i<IndexNum; i++)
		if(IndexName == IndexSet[i].IndexFileName)
		{
			index = i;
			break;
		}

	if(Key_Type == INT_TYPE)
		succeed = IndexSet[i].B_Plus_Tree_int.Delete(stoi(KeyValue));
	else if(Key_Type == FLOAT_TYPE)
		succeed = IndexSet[i].B_Plus_Tree_float.Delete(stof(KeyValue));
	else if(Key_Type == STRING_TYPE)
		succeed = IndexSet[i].B_Plus_Tree_string.Delete(KeyValue);
	else
		succeed = false;
	
	return succeed;
}



void IndexManager::writeToDisk()
{

	string IndexName;	
	IndexName == IndexSet[0].IndexFileName;		
	if (IndexName != "")
	{
		DeleteIndex(IndexName);
		bm_y.writeBackDisk(IndexName);
	}

}

/********************************/
void IndexManager::levelListIndex(string IndexName, int Key_Type)
{
	int i = 0;
	int index = 0;
	if (IndexSet[0].IndexFileName != IndexName)
	{
		if (IndexSet[0].IndexFileName != "")
		{
			bm_y.writeBackDisk(IndexSet[0].IndexFileName);
		}
		bm_y.loadToArray(IndexName);
		GetIndex(IndexName, Key_Type);

	}
	for (i = 0; i<IndexNum; i++)
		if (IndexName == IndexSet[i].IndexFileName)
		{
			index = i;
			break;
		}
	IndexSet[i].B_Plus_Tree_int.Level_List();

}

bool IndexManager::SearchInRange(string IndexName, int Key_Type, string min_KeyValue, bool isLeftEqual,
	string max_KeyValue, bool isRightEqual,  vector<int> &return_result)
{
	int i;
	int index = -1;

	bool exist = false;
	if (IndexSet[0].IndexFileName != IndexName)
	{
		if (IndexSet[0].IndexFileName != "")
		{
			bm_y.writeBackDisk(IndexSet[0].IndexFileName);
			DeleteIndex(IndexSet[0].IndexFileName);
		}
		bm_y.loadToArray(IndexName);
		GetIndex(IndexName, Key_Type);

	}

	for (i = 0; i<IndexNum; i++)
		if (IndexName == IndexSet[i].IndexFileName)
		{
			index = i;
			break;
		}

	if (index == -1)
	{
		//cout<<"No such index named "<<IndexName<<" ,Search failed!"<<endl;
		return -1;
	}
	else
	{
		if (Key_Type == INT_TYPE)
		{
			IndexSet[i].B_Plus_Tree_int.SearchRange(stoi(min_KeyValue), isLeftEqual, stoi(max_KeyValue), isRightEqual, return_result);
			if (return_result[0] > 0)
			{
				exist = true;
			}
		}
		else if (Key_Type == FLOAT_TYPE)
		{
			IndexSet[i].B_Plus_Tree_float.SearchRange(stof(min_KeyValue), isLeftEqual, stof(max_KeyValue), isRightEqual, return_result);
			if (return_result[0] > 0)
			{
				exist = true;
			}
		}
		else if (Key_Type == STRING_TYPE)
		{
			IndexSet[i].B_Plus_Tree_string.SearchRange(min_KeyValue, isLeftEqual, max_KeyValue, isRightEqual, return_result);
			if (return_result[0] > 0)
			{
				exist = true;
			}
		}
		else
			exist = false;
	}

	if (exist)
		return true;
	else
		return -1;
}