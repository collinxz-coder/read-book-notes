/**
*Copyright (c) 2000-2001, Jim Crafton
*All rights reserved.
*Redistribution and use in source and binary forms, with or without
*modification, are permitted provided that the following conditions
*are met:
*	Redistributions of source code must retain the above copyright
*	notice, this list of conditions and the following disclaimer.
*
*	Redistributions in binary form must reproduce the above copyright
*	notice, this list of conditions and the following disclaimer in 
*	the documentation and/or other materials provided with the distribution.
*
*THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
*AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS
*OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
*PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
*NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
*SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*NB: This software will not save the world.
*/

/* Generated by Together */



#ifndef TREEMODEL_H
#define TREEMODEL_H


namespace VCF{

class TreeItem;

class TreeModelListener;

/** @interface */

#define TREEMODEL_CLASSID		"3126B225-2819-11d4-B53A-00C04F0196DA"
class APPKIT_API TreeModel : public Model {
public:
	
	BEGIN_ABSTRACT_CLASSINFO(TreeModel, "VCF::TreeModel", "VCF::Model", TREEMODEL_CLASSID)
//	OBJECT_PROPERTY( TreeItem, "root", TreeModel::getRoot, TreeModel::setRoot );
	EVENT( "VCF::TreeModelEventHandler", "VCF::TreeModelEvent", "RootNodeChanged" )
	EVENT( "VCF::TreeModelEventHandler", "VCF::TreeModelEvent", "NodeAdded" )
	EVENT( "VCF::TreeModelEventHandler", "VCF::TreeModelEvent", "NodeDeleted" )
	END_CLASSINFO(TreeModel)

	TreeModel(){
		
	};


	virtual ~TreeModel(){};

    virtual Enumerator<TreeItem*>* getRootItems() = 0;

	virtual void addTreeRootNodeChangedHandler( EventHandler* handler ) = 0;

	virtual void removeTreeRootNodeChangedHandler( EventHandler* handler ) = 0;

	virtual void addTreeNodeAddedHandler( EventHandler* handler ) = 0;

	virtual void removeTreeNodeAddedHandler( EventHandler* handler ) = 0;

	virtual void addTreeNodeDeletedHandler( EventHandler* handler ) = 0;

	virtual void removeTreeNodeDeletedHandler( EventHandler* handler ) = 0;

    virtual void insertNodeItem( TreeItem * node, TreeItem * nodeToInsertAfter ) = 0;


    virtual void deleteNodeItem( TreeItem * nodeToDelete ) = 0;

    virtual void addNodeItem( TreeItem * node, TreeItem * nodeParent=NULL ) = 0;

    virtual void sort() = 0;
};

};

#endif //TREEMODEL_H