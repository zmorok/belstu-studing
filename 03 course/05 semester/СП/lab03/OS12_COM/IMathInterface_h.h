

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 06:14:07 2038
 */
/* Compiler settings for IMathInterface.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IMathInterface_h_h__
#define __IMathInterface_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IAdder_FWD_DEFINED__
#define __IAdder_FWD_DEFINED__
typedef interface IAdder IAdder;

#endif 	/* __IAdder_FWD_DEFINED__ */


#ifndef __IMultiplier_FWD_DEFINED__
#define __IMultiplier_FWD_DEFINED__
typedef interface IMultiplier IMultiplier;

#endif 	/* __IMultiplier_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IAdder_INTERFACE_DEFINED__
#define __IAdder_INTERFACE_DEFINED__

/* interface IAdder */
/* [uuid][object] */ 


EXTERN_C const IID IID_IAdder;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("88E95996-C6B0-4859-A641-A0B14748068F")
    IAdder : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Add( 
            const double x,
            const double y,
            double z) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Sub( 
            const double x,
            const double y,
            double z) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IAdderVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdder * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdder * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdder * This);
        
        DECLSPEC_XFGVIRT(IAdder, Add)
        HRESULT ( STDMETHODCALLTYPE *Add )( 
            IAdder * This,
            const double x,
            const double y,
            double z);
        
        DECLSPEC_XFGVIRT(IAdder, Sub)
        HRESULT ( STDMETHODCALLTYPE *Sub )( 
            IAdder * This,
            const double x,
            const double y,
            double z);
        
        END_INTERFACE
    } IAdderVtbl;

    interface IAdder
    {
        CONST_VTBL struct IAdderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdder_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAdder_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAdder_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAdder_Add(This,x,y,z)	\
    ( (This)->lpVtbl -> Add(This,x,y,z) ) 

#define IAdder_Sub(This,x,y,z)	\
    ( (This)->lpVtbl -> Sub(This,x,y,z) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAdder_INTERFACE_DEFINED__ */


#ifndef __IMultiplier_INTERFACE_DEFINED__
#define __IMultiplier_INTERFACE_DEFINED__

/* interface IMultiplier */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMultiplier;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6114BED6-539E-4CD6-9FEC-B877970C5BE3")
    IMultiplier : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Mul( 
            const double x,
            const double y,
            double z) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Div( 
            const double x,
            const double y,
            double z) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMultiplierVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMultiplier * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMultiplier * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMultiplier * This);
        
        DECLSPEC_XFGVIRT(IMultiplier, Mul)
        HRESULT ( STDMETHODCALLTYPE *Mul )( 
            IMultiplier * This,
            const double x,
            const double y,
            double z);
        
        DECLSPEC_XFGVIRT(IMultiplier, Div)
        HRESULT ( STDMETHODCALLTYPE *Div )( 
            IMultiplier * This,
            const double x,
            const double y,
            double z);
        
        END_INTERFACE
    } IMultiplierVtbl;

    interface IMultiplier
    {
        CONST_VTBL struct IMultiplierVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMultiplier_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMultiplier_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMultiplier_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMultiplier_Mul(This,x,y,z)	\
    ( (This)->lpVtbl -> Mul(This,x,y,z) ) 

#define IMultiplier_Div(This,x,y,z)	\
    ( (This)->lpVtbl -> Div(This,x,y,z) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMultiplier_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


