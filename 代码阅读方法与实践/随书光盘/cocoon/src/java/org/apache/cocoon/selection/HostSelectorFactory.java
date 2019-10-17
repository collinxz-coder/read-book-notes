/*****************************************************************************
 * Copyright (C) The Apache Software Foundation. All rights reserved.        *
 * ------------------------------------------------------------------------- *
 * This software is published under the terms of the Apache Software License *
 * version 1.1, a copy of which has been included  with this distribution in *
 * the LICENSE file.                                                         *
 *****************************************************************************/
package org.apache.cocoon.selection;

import org.apache.avalon.framework.activity.Initializable;

/**
 * @version CVS $Revision: 1.2 $ $Date: 2002/01/12 01:14:34 $
 * @deprecated replaced by HostSelector - code factories should no longer be used
 */

public class HostSelectorFactory extends HostSelector implements Initializable {

    public void initialize() {
        getLogger().warn("HostSelectorFactory is deprecated. Please use HostSelector");
    }
}
