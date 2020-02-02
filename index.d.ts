export function get(path: string, attr: string): Promise<Buffer>;
export function getSync(path: string, attr: string): Buffer;

export function set(path: string, attr: string, value: Buffer): Promise<void>;
export function setSync(path: string, attr: string, value: Buffer): string;

export function remove(path: string, attr: string): Promise<void>;
export function removeSync(path: string, attr: string): string;

export function list(path: string, attr: string): Promise<string[]>;
export function listSync(path: string, attr: string): string[];
