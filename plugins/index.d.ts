declare module '@ejs/core' {
    export interface Plugin {
        on: (eventName: string, handler: () => boolean | undefined) => void;
    }

    export const plugin: Plugin;
}