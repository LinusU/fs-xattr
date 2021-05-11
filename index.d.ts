/**
 * Get extended attribute `attr` from file at `path`.
 *
 * @returns a `Promise` that will resolve with the value of the attribute.
 */
export function getAttribute (path: string, attr: string): Promise<Buffer>

/**
 * Synchronous version of `getAttribute`.
 */
export function getAttributeSync (path: string, attr: string): Buffer

/**
 * Set extended attribute `attr` to `value` on file at `path`.
 *
 * @returns a `Promise` that will resolve when the value has been set.
 */
export function setAttribute (path: string, attr: string, value: Buffer | string): Promise<void>

/**
 * Synchronous version of `setAttribute`.
 */
export function setAttributeSync (path: string, attr: string, value: Buffer | string): void

/**
 * Remove extended attribute `attr` on file at `path`.
 *
 * @returns a `Promise` that will resolve when the value has been removed.
 */
export function removeAttribute (path: string, attr: string): Promise<void>

/**
 * Synchronous version of `removeAttribute`.
 */
export function removeAttributeSync (path: string, attr: string): void

/**
 * List all attributes on file at `path`.
 *
 * @returns a `Promise` that will resolve with an array of strings, e.g. `['user.linusu.test', 'com.apple.FinderInfo']`.
 */
export function listAttributes (path: string): Promise<string[]>

/**
 * Synchronous version of `listAttributes`.
 */
export function listAttributesSync (path: string): string[]
