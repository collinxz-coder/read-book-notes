//Win32ScintillaPeer.h

#ifndef _SCINTILLAPEER_H__
#define _SCINTILLAPEER_H__



#include "AbstractWin32Component.h"
#include "TextPeer.h"

using namespace VCF;


namespace VCFBuilder  {

class CodeEditor;

typedef int (*ScintillaFunc)(void*,int,int,int);

/**
*Class Win32ScintillaPeer documentation
*/
class Win32ScintillaPeer : public VCF::AbstractWin32Component, public VCF::TextPeer { 
public:
	Win32ScintillaPeer( CodeEditor* editorControl );

	virtual ~Win32ScintillaPeer();

	/**
     * returns a ID that identifies this control. On Win32 this corresponds to an HWND. 
     */
    virtual long getHandleID(){
		return AbstractWin32Component::getHandleID();	
	};

    /**
     * returns a text associated with the component. This usually gets used in the Component::getCaption() method. 
     */
    virtual VCF::String getText();

    /**
     * sets the text for the widget 
     */
    virtual void setText( const VCF::String& text );

    /**
     * sets the bounds for the component. Bounds are specified in the coordinate system of the componenents parent. 
     */
    virtual void setBounds( VCF::Rect* rect ){
		AbstractWin32Component::setBounds( rect );	
	};

    /**
     * returns the bounds of the component in the coordinate system of the parent. 
     */
    virtual VCF::Rect* getBounds(){
		return AbstractWin32Component::getBounds();	
	};

    /**
     * shows or hides the component. 
     * This does NOT close the component (if invoked on a frame based component ). 
     */
    virtual void setVisible( const bool& visible ){
		AbstractWin32Component::setVisible( visible );	
	};

    /**
     * returns wether or not the component is currently visible. 
     */
    virtual bool getVisible(){
		return AbstractWin32Component::getVisible();	
	};

    /**
     * returns a bit-masked unsigned long that contains style constants. 
     *  These style constants are defined in the VCF, and must 
     * be translated to the particular windowing system being used. 
     */
    virtual unsigned long getStyleMask(){
		return AbstractWin32Component::getStyleMask();	
	};

    /**
     * sets the current style mask.
     *  Should cause a repaint of the component, if neccessary. 
     */
    virtual void setStyleMask( const unsigned long& styleMask ){
		AbstractWin32Component::setStyleMask( styleMask );	
	};

    /**
     * returns the component that this Peer is attached to. 
     */
    virtual VCF::Control* getControl(){
		return AbstractWin32Component::getControl();	
	};

    /**
     * attahces the Peer to a particular component. This should only be done once. 
     */
    virtual void setControl( VCF::Control* component );

    virtual void setParent( VCF::Control* parent ){
		AbstractWin32Component::setParent( parent );	
	};

	virtual VCF::Control* getParent(){
		return AbstractWin32Component::getParent();	
	};

	virtual bool isFocused(){
		return AbstractWin32Component::isFocused();	
	};

	virtual void setFocus( const bool& focused ){
		AbstractWin32Component::setFocus( focused );
	};

	virtual bool isEnabled(){
		return AbstractWin32Component::isEnabled();	
	};

	virtual void setEnabled( const bool& enabled ){
		AbstractWin32Component::setEnabled( enabled );	
	};

	virtual void setFont( Font* font ){
		AbstractWin32Component::setFont( font );	
	};

	virtual void repaint( Rect* repaintRect=NULL ){
		AbstractWin32Component::repaint(repaintRect);
	};
	
	virtual void keepMouseEvents(){
		AbstractWin32Component::keepMouseEvents();
	};
	
	virtual void releaseMouseEvents(){
		AbstractWin32Component::releaseMouseEvents();
	};

	virtual void setCursor( Cursor* cursor ) {
		AbstractWin32Component::setCursor( cursor );
	}
	WNDPROC getScintillaWndProc() {
		return m_scintillaWndProc;
	}
	virtual LRESULT handleEventMessages( UINT message, WPARAM wParam, LPARAM lParam, WNDPROC defaultWndProc = NULL );

	virtual void setRightMargin( const double & rightMargin );
    
	virtual void setLeftMargin( const double & leftMargin );

    virtual unsigned long getLineCount();

    virtual void getCurrentLinePosition();

    virtual double getLeftMargin();

    virtual double getRightMargin();

    virtual Point* getPositionFromCharIndex( const unsigned long& index );

    virtual unsigned long getCharIndexFromPosition( Point* point );

	/**
	*returns the current caret position with in the text control
	*this is specified by a zero based number representing the 
	*insertion point with the text control's text (stored in the text
	*control's Model).
	*@return long the index of the current insertion point in the Model's text
	*/
	virtual unsigned long getCaretPosition();

	void onTextModelTextChanged( TextEvent* event );
protected:
	CodeEditor* m_editorControl;
	ScintillaFunc m_scintillaFunc;
	void* m_scintillaObject;

	void setEditorStyle( const int& style, const ulong32& foreColor, const ulong32& backColor, const ulong32& size, const String& fontFace, const bool& bold=false, const bool& italic=false );

	void braceMatch();

	bool findMatchingBracePosition( int &braceAtCaret, int &braceOpposite, const bool& sloppy );
private:
	WNDPROC m_scintillaWndProc;
};


}; //end of namespace VCFBuilder

#endif //_SCINTILLAPEER_H__


