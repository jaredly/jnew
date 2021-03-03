import { triple } from './Shared';

export type config = {
    title: string;
    tags: string[];
    categories: string[];
    date: triple;
    description?: string;
    thumbnail?: string;
    article_image?: string;
    featured: boolean;
    draft: boolean;
    fileName: string;
    wordCount?: number;
};
