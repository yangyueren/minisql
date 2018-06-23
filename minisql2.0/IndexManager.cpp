#include "IndexManager.h"

using namespace std;

IndexManager::IndexManager()
{
	IndexNum = 1;
}

IndexManager::~IndexManager()
{

}

bool IndexManager::GetIndex(string IndexName, int Key_Type)
{
	struct Index temp_index;

	temp_index.Type = Key_Type;
	temp_index.IndexFileName = IndexName;
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
		cout<<"Index "<<IndexName<<" has been deleted!"<<endl;
		return true;
	}
}

bool IndexManager::CreateIndex(string IndexName, int KeySize, int Key_Type, int Degree)
{
	
	if(Key_Type == INT_TYPE)
	{
		BPlusTree<int> BPT(IndexName, KeySize, Key_Type, Degree);
		//UpdateIndex(&BPT);
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

	return true;
}


bool IndexManager::CreateIndex(string IndexName, int KeySize, int Key_Type)
{
	int Degree;

	Degree = 4000/KeySize;
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
		GetIndex(IndexName, Key_Type);
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
		GetIndex(IndexName, Key_Type);
	}

	for(i=0; i<IndexNum; i++)
		if(IndexName == IndexSet[i].IndexFileName)
		{
			index = i;
			break;
		}
	/*
	if(index == -1)
	{
		cout<<"No such index named "<<IndexName<<" ,deletion failed!"<<endl;
		return -1;
	}
	else
	{
		if(Key_Type == INT_TYPE)
			exist = IndexSet[i].B_Plus_Tree_int.Search(stoi(KeyValue), offset);
		else if(Key_Type == FLOAT_TYPE)
			exist = IndexSet[i].B_Plus_Tree_float.Search(stof(KeyValue), offset);
		else if(Key_Type == STRING_TYPE)
			exist = IndexSet[i].B_Plus_Tree_string.Search(KeyValue, offset);
		else
			exist = false;
	}	
	*/
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