#ifndef  _SO_VTK_RENDER_ACTION_
#define  _SO_VTK_RENDER_ACTION_

#include <Inventor/actions/SoGLRenderAction.h>

class	SbBox3f;
class	SoGetBoundingBoxAction;

// Callback functions used between rendering passes should be of this type.
typedef void	SoGLRenderPassCB(void *userData);

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoVTKRenderAction
//
//  GL rendering action.
//
//////////////////////////////////////////////////////////////////////////////

// C-api: prefix=SoGLRenderAct
class /*INVENTOR_API*/ SoVTKRenderAction : public SoGLRenderAction {

    SO_ACTION_HEADER(SoVTKRenderAction);

  public:
    enum VTKRenderPassType {
	RenderOpaqueGeometry,
	RenderTranslucentPolygonalGeometry,
	RenderVolumetricGeometry,
	RenderOverlay
    };

    // Constructor. The  parameter defines the viewport region
    // into which rendering will take place. 
    SoVTKRenderAction(const SbViewportRegion &viewportRegion);
		
    // Destructor
    virtual ~SoVTKRenderAction();

    // Applies action to the graph rooted by a node
    // C-api: expose
    virtual void	apply(SoNode *node);

    // Applies action to the graph defined by a path
    // C-api: expose
    // C-api: name=ApplyPath
    virtual void	apply(SoPath *path);

    // Applies action to the graphs defined by a path list. The
    // "obeysRules" flag can be set to TRUE if the given path list has
    // the following 4 properties:
    //    1 - All paths have the same head node
    //    2 - Paths are sorted in traversal order
    //    3 - If one path ends at node A, no other path continues through A
    //	  4 - No two paths are the same
    // These rules will be obeyed by path lists returned by searches
    // for non-group nodes.
    // C-api: expose
    // C-api: name=ApplyPathList
    virtual void	apply(const SoPathList &pathList,
			      SbBool obeysRules = FALSE);

    // Sets/returns vtkRenderPassType to use when rendering
    // C-api: name=setVTKRenderPassType
    void		setVTKRenderPassType(VTKRenderPassType type);
    // C-api: name=getVTKRenderPassType
    VTKRenderPassType	getVTKRenderPassType() const	{ return vtkRenderPassType; }

  protected:
    // Initiates action on graph
    //virtual void	beginTraversal(SoNode *node);

    VTKRenderPassType vtkRenderPassType;
};

#endif /* _SO_VTK_RENDER_ACTION_ */
