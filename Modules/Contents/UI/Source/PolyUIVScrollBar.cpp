/*
 Copyright (C) 2012 by Ivan Safrin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */


#include "PolyUIVScrollBar.h"
#include "PolyConfig.h"
#include "PolyInputEvent.h"
#include "PolyLabel.h"
#include "PolyCoreServices.h"

using namespace Polycode;

UIVScrollBar::UIVScrollBar(Number width, Number height, Number initialRatio) : ScreenEntity() {

	scrollValue = 0;
	
	Config *conf = CoreServices::getInstance()->getConfig();	
	
	minHandleSize = conf->getNumericValue("Polycode", "uiScrollHandleMinSize");
	
	Number st = conf->getNumericValue("Polycode", "uiScrollBgSkinT");
	Number sr = conf->getNumericValue("Polycode", "uiScrollBgSkinR");
	Number sb = conf->getNumericValue("Polycode", "uiScrollBgSkinB");
	Number sl = conf->getNumericValue("Polycode", "uiScrollBgSkinL");	
	padding = conf->getNumericValue("Polycode", "uiScrollBgSkinPadding");		
		
	bgBox = new UIBox(conf->getStringValue("Polycode", "uiScrollBgSkin"),
					  st,sr,sb,sl,
					  width, height);
	
	addChild(bgBox);
	
	st = conf->getNumericValue("Polycode", "uiScrollHandleSkinT");
	sr = conf->getNumericValue("Polycode", "uiScrollHandleSkinR");
	sb = conf->getNumericValue("Polycode", "uiScrollHandleSkinB");
	sl = conf->getNumericValue("Polycode", "uiScrollHandleSkinL");	
	
	if(initialRatio > 1)
		initialRatio = 1;
	
	scrollHandleHeight = height*initialRatio;	
	
	if(scrollHandleHeight < minHandleSize)
		scrollHandleHeight = minHandleSize;
	
	handleBox = new UIBox(conf->getStringValue("Polycode", "uiScrollHandleSkin"),
					  st,sr,sb,sl,
					  width-(padding*2),scrollHandleHeight);
	handleBox->setPosition(padding, padding);	
	addChild(handleBox);
	
	bgBox->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);
	
	handleBox->addEventListener(this, InputEvent::EVENT_MOUSEUP);
	handleBox->addEventListener(this, InputEvent::EVENT_MOUSEUP_OUTSIDE);	
	handleBox->addEventListener(this, InputEvent::EVENT_MOUSEDOWN);			
	
	handleBox->blockMouseInput = true;
	
	dragRectHeight = height-(padding*2)-scrollHandleHeight;
	handleBox->setDragLimits(Rectangle(padding,padding,width-(padding*2)-(width-(padding*2)), dragRectHeight));
	
	lastPositionY = handleBox->getPosition().y;
	
	this->height = height;
	this->width = width;	
	
	this->hitwidth = width;
	this->hitheight = height;	
}

void UIVScrollBar::Resize(int newHeight) {
	bgBox->resizeBox(width, newHeight);
	this->height = newHeight;
	this->hitheight = newHeight;
	dragRectHeight = height-(padding*2)-scrollHandleHeight;	
	handleBox->setDragLimits(Rectangle(padding,padding,width-(padding*2)-(width-(padding*2)), dragRectHeight));	
}

void UIVScrollBar::Update() {
	
	if(lastPositionY != handleBox->getPosition().y) {
		lastPositionY = handleBox->getPosition().y;
		scrollValue = (lastPositionY-padding)/dragRectHeight;
		if(scrollValue < 0) scrollValue = 0;
		if(scrollValue > 1) scrollValue = 1;		
		dispatchEvent(new Event(), Event::CHANGE_EVENT);
	}
}

void UIVScrollBar::setHandleRatio(Number newRatio) {
	scrollHandleHeight = height*newRatio;	
	
	if(scrollHandleHeight < minHandleSize)
		scrollHandleHeight = minHandleSize;

	dragRectHeight = height-(padding*2)-scrollHandleHeight;	
	handleBox->resizeBox(handleBox->getWidth(), scrollHandleHeight);
	handleBox->setDragLimits(Rectangle(padding,padding,width-(padding*2)-(width-(padding*2)), dragRectHeight));	
}

void UIVScrollBar::onMouseWheelUp(Number x, Number y) {
	scrollUpOneTick();
}

void UIVScrollBar::onMouseWheelDown(Number x, Number y) {
	scrollDownOneTick();
}

void UIVScrollBar::scrollUpOneTick() {
	Number newPos = handleBox->getPosition().y - 5;
	
	if(newPos < padding)
		newPos = padding;
	
	handleBox->setPositionY(newPos);	
}

void UIVScrollBar::scrollDownOneTick() {
	Number newPos = handleBox->getPosition().y + 5;	
	
	if(newPos > dragRectHeight)
		newPos = dragRectHeight;
	
	handleBox->setPositionY(newPos);	
}


Number UIVScrollBar::getScrollValue() {
	return scrollValue;
}

void UIVScrollBar::handleEvent(Event *event) {
	if(event->getDispatcher() == bgBox) {
		InputEvent *inputEvent = (InputEvent*)event;
		switch(event->getEventCode()) {
			case InputEvent::EVENT_MOUSEDOWN:
				if(inputEvent->mousePosition.y < handleBox->getPosition().y)  {
					Number newPos = handleBox->getPosition().y - scrollHandleHeight;
					if(newPos < padding)
						newPos = padding;
					handleBox->setPositionY(newPos);
				} else {
					Number newPos = handleBox->getPosition().y + scrollHandleHeight;
					if(newPos > dragRectHeight)
						newPos = dragRectHeight;
					handleBox->setPositionY(newPos);					
				}
			break;
		}
	}
	
	if(event->getDispatcher() == handleBox) {
		InputEvent *inputEvent = (InputEvent*)event;
		switch(event->getEventCode()) {
			case InputEvent::EVENT_MOUSEUP:
			case InputEvent::EVENT_MOUSEUP_OUTSIDE:				
				handleBox->stopDrag();
				break;
			case InputEvent::EVENT_MOUSEDOWN:
				handleBox->startDrag(inputEvent->mousePosition.x-handleBox->getPosition().x,inputEvent->mousePosition.y-handleBox->getPosition().y);
				break;		
		}
	}
}
	

UIVScrollBar::~UIVScrollBar() {
	
}
