

#include <float.h>
#include <Inventor/SoDB.h>
#include "SoVTKRenderAction.h"
#include OPENGL_GL_H

SO_ACTION_SOURCE(SoVTKRenderAction);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor. The first parameter defines the viewport region
//    into which rendering will take place. 
//
// Use: public

SoVTKRenderAction::SoVTKRenderAction(const SbViewportRegion &viewportRegion) 
  : SoGLRenderAction(viewportRegion), vtkRenderPassType(RenderOpaqueGeometry)
//
////////////////////////////////////////////////////////////////////////
{
    //SO_ACTION_CONSTRUCTOR(SoVTKRenderAction);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: public

SoVTKRenderAction::~SoVTKRenderAction()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Applies action to the graph rooted by a node.
//
// Use: public

void
SoVTKRenderAction::apply(SoNode *node)
//
////////////////////////////////////////////////////////////////////////
{
  SoGLRenderAction::apply(node);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Applies action to the graph defined by a path.
//
// Use: public

void
SoVTKRenderAction::apply(SoPath *path)
//
////////////////////////////////////////////////////////////////////////
{
  if( (getVTKRenderPassType()==RenderOpaqueGeometry) && (isDelaying()) )
    SoGLRenderAction::apply(path);
  else
  if( (getVTKRenderPassType()==RenderOverlay) && (isRenderingDelayedPaths()) )
    SoGLRenderAction::apply(path);
  else
  if( (getVTKRenderPassType()==RenderTranslucentPolygonalGeometry) && (!isRenderingDelayedPaths()) )
    SoGLRenderAction::apply(path);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Applies action to the graphs defined by a path list.
//
// Use: public

void
SoVTKRenderAction::apply(const SoPathList &pathList, SbBool obeysRules)
//
////////////////////////////////////////////////////////////////////////
{
  if( (getVTKRenderPassType()==RenderOpaqueGeometry) && (isDelaying()) )
    SoGLRenderAction::apply(pathList, obeysRules);
  else
  if( (getVTKRenderPassType()==RenderOverlay) && (isRenderingDelayedPaths()) )
    SoGLRenderAction::apply(pathList, obeysRules);
  else
  if( (getVTKRenderPassType()==RenderTranslucentPolygonalGeometry) && (!isRenderingDelayedPaths()) )
    SoGLRenderAction::apply(pathList, obeysRules);
}

void
SoVTKRenderAction::setVTKRenderPassType(VTKRenderPassType type)
{
  this->vtkRenderPassType = type;
}
  
//void
//SoVTKRenderAction::beginTraversal(SoNode *node)
////
//////////////////////////////////////////////////////////////////////////
//{
//    // This is called either from the main call to apply() that is
//    // used to render a graph OR from the apply() call made while
//    // rendering transparent objects or delayed objects. In the first
//    // case, we want to render all passes. In the second and third
//    // cases, we want to render only the current pass. We can tell
//    // these cases apart by examining the flags.
//
//    if (renderingTranspObjs || renderingDelPaths)
//	traverse(node);
//
//    else
//	renderAllPasses(node);
//}
//

//////////////////////////////////////////////////////////////////////////
////
//// Description:
////    Does one pass of rendering starting at a node.
////
//// Use: private
//
//void
//SoVTKRenderAction::renderPass(SoNode *node, int pass)
////
//////////////////////////////////////////////////////////////////////////
//{
//    // Set the current pass in the instance and in the state
//    curPass = pass;
//    SoGLRenderPassElement::set(getState(), pass);
//
//    // Set the viewport region 
//    SoViewportRegionElement::set(getState(), vpRegion);
//    SoGLUpdateAreaElement::set(getState(), updateOrigin, updateSize);
//
//    // Do the actual rendering
//    traverse(node);
//
//    // For delayed (or sorted) transparency, see if any transparent
//    // objects were added
//    if (delayObjs && transpPaths.getLength() > 0 && ! hasTerminated()) {
//
//	// Make sure blending is enabled if necessary
//	if (transpType != SCREEN_DOOR)
//	    enableBlending(TRUE);
//
//	renderTransparentObjs();
//
//	// Disable blending for next pass
//	if (transpType != SCREEN_DOOR)
//	    enableBlending(FALSE);
//    }
//
//    // Delayed paths
//    if (delayedPaths.getLength() > 0 && ! hasTerminated()) {
//	renderingDelPaths = TRUE;
//
//	// Render paths to delayed objects. We know these paths obey
//	// the rules for compact path lists, so let the action know
//	apply(delayedPaths, TRUE);
//
//	// Clear out the list
//	delayedPaths.truncate(0);
//
//	renderingDelPaths = FALSE;
//    }
//}
//
//////////////////////////////////////////////////////////////////////////
////
//// Description:
////    Renders delayed objects that have been marked as transparent.
////    This sorts them if necessary. This should be called only if we
////    are delaying transparent objects and there is at least one
////    transparent object.
////
//// Use: private
//
//void
//SoVTKRenderAction::renderTransparentObjs()
////
//////////////////////////////////////////////////////////////////////////
//{
//    int	i, numObjs = transpPaths.getLength(), numToDo;
//
//    // Indicate that we are doing transparent objects so we know not
//    // to render all passes
//    renderingTranspObjs = TRUE;
//
//    // Indicate that objects are not to be delayed. (So they will render.)
//    delayObjs = FALSE;
//
//    // Don't write into z buffer so that ALL transparent objects will
//    // be drawn. This makes things look better, even if sorting is not
//    // on or if sorting gives the incorrect order.
//    glDepthMask(FALSE);
//
//    // If not sorting, just render them in order
//    if (! sortObjs)
//	// Render paths to transparent objects. We know these paths
//	// obey the rules for compact path lists, so let the action know.
//	apply(transpPaths, TRUE);
//
//    // Otherwise, compute bounding boxes, render objs back to front
//    else {
//	if (ba == NULL) {
//	    ba = new SoGetBoundingBoxAction(vpRegion);
//
//	    // Make sure bounding boxes are in camera space. This
//	    // means the z coordinates of the bounding boxes indicate
//	    // distance from the camera.
//	    ba->setInCameraSpace(TRUE);
//	}
//
//	// Make sure there is room for the bounding boxes
//	if (bboxes == NULL) {
//	    bboxes = new SbBox3f[numObjs];
//	    numBBoxes = numObjs;
//	}
//	else if (numBBoxes < numObjs) {
//	    delete [] bboxes;
//	    bboxes = new SbBox3f[numObjs];
//	    numBBoxes = numObjs;
//	}
//
//	for (i = 0; i < numObjs; i++) {
//	    ba->apply(transpPaths[i]);
//	    bboxes[i] = ba->getBoundingBox();
//	}
//
//	// Render them in sorted order
//	for (numToDo = numObjs; numToDo > 0; --numToDo) {
//	    int		farthest;
//	    float	zFar;
//
//	    // Use selection sort, since number of objects is usually small
//
//	    // Look for bbox with smallest zmax (farthest from camera!)
//	    zFar = FLT_MAX;
//	    for (i = 0; i < numObjs; i++) {
//		if (bboxes[i].getMax()[2] < zFar) {
//		    zFar = bboxes[i].getMax()[2];
//		    farthest = i;
//		}
//	    }
//
//	    // Render farthest one
//	    apply(transpPaths[farthest]);
//
//	    // Mark it as being far
//	    bboxes[farthest].getMax()[2] = FLT_MAX;
//	}
//    }
//
//    // Restore zwritemask to what we assume it was before...
//    glDepthMask(TRUE);
//
//    // Get ready for next time
//    delayObjs = TRUE;
//    transpPaths.truncate(0);
//    renderingTranspObjs = FALSE;
//}

